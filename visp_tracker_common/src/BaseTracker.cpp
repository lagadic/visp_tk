#include <visp_tracker_common/BaseTracker.hpp>

namespace visp_tracker_common
{
const std::string BaseTracker::s_dumb_topic_name = "dumb_topic_name";

const unsigned int BaseTracker::s_default_hor_offset = 180;

BaseTracker::BaseTracker(const std::string &node_name, const bool &does_publish_features) : rclcpp::Node(node_name)
{
  //////////////////////////////////////////////////////////////////////
  //                        ROS2 PARAMETERS                           //
  //////////////////////////////////////////////////////////////////////
  auto headless_mode_param = rclcpp::Parameter();
  auto headless_mode_param_desc = rcl_interfaces::msg::ParameterDescriptor {};
  headless_mode_param_desc.description = "If true, the node will not display anything, expecting that another node takes in charge the GUI.";
  this->declare_parameter("headless_mode", false, headless_mode_param_desc);
  this->get_parameter("headless_mode", headless_mode_param);
  m_is_headless_mode = headless_mode_param.as_bool();

  auto display_nb_frames_skipped_param = rclcpp::Parameter();
  auto display_nb_frames_skipped_param_desc = rcl_interfaces::msg::ParameterDescriptor {};
  display_nb_frames_skipped_param_desc.description = "This parameter indicates the number of frames skipped during display";
  this->declare_parameter("display_nb_frames_skipped", -1, display_nb_frames_skipped_param_desc);
  this->get_parameter("display_nb_frames_skipped", display_nb_frames_skipped_param);
  m_display_nb_frames_skipped = display_nb_frames_skipped_param.as_int();

  auto vis_debug_param = rclcpp::Parameter();
  auto vis_debug_param_desc = rcl_interfaces::msg::ParameterDescriptor {};
  vis_debug_param_desc.description = "When this parameter is set to true, the node will publish the 3D position of the joints and of the desired 3D position as MarkerArray message.";
  this->declare_parameter("visualization_debug", false, vis_debug_param_desc);
  this->get_parameter("visualization_debug", vis_debug_param);
  m_visualization_debug = vis_debug_param.as_bool();

  auto config_file_param = rclcpp::Parameter();
  auto config_file_desc = rcl_interfaces::msg::ParameterDescriptor {};
  config_file_desc.description = "If set, path to the configuration file to initialize the tracker. package:// will be replaced by the path to the share folder of the corresponding package.";
  this->declare_parameter("config_file", "", config_file_desc);
  this->get_parameter("config_file", config_file_param);
  m_config_file = visp_common::path::path_retriever(config_file_param.as_string());

  // // ---- Parameters related to the services ----

  // // ---- Parameters related to the publishers / subscribers ----
  auto camera_topic_name_param = rclcpp::Parameter();
  auto camera_topic_name_desc = rcl_interfaces::msg::ParameterDescriptor {};
  camera_topic_name_desc.description = "Name of the (reference) color camera topic.";
  this->declare_parameter("rgb_camera_topic_name", "", camera_topic_name_desc);
  this->get_parameter("rgb_camera_topic_name", camera_topic_name_param);
  m_rgb_camera_topic_name = camera_topic_name_param.as_string();
  if (m_rgb_camera_topic_name.empty()) {
    RCLCPP_ERROR(this->get_logger(), "'rgb_camera_topic_name' has not been set ! Setting a dumb value.");
    m_rgb_camera_topic_name = s_dumb_topic_name;
  }

  auto rgb_stream_topic_name_param = rclcpp::Parameter();
  auto rgb_stream_topic_name_desc = rcl_interfaces::msg::ParameterDescriptor {};
  rgb_stream_topic_name_desc.description = "Name of the (reference) color image topic.";
  this->declare_parameter("rgb_stream_topic_name", "", rgb_stream_topic_name_desc);
  this->get_parameter("rgb_stream_topic_name", rgb_stream_topic_name_param);
  m_rgb_stream_name = rgb_stream_topic_name_param.as_string();
  if (m_rgb_stream_name.empty()) {
    RCLCPP_ERROR(this->get_logger(), "'rgb_stream_topic_name' has not been set ! Setting a dumb value.");
    m_rgb_stream_name = s_dumb_topic_name;
  }

  // // ---- Parameters changes handling ----

  // Create a parameter subscriber that can be used to monitor parameter changes
  // (for this node's parameters as well as other nodes' parameters)
  param_subscriber_ = std::make_shared<rclcpp::ParameterEventHandler>(this);

  // Set a callback for this node's integer parameter, "display_nb_frames_skipped"
  auto cb = [this](const rclcpp::Parameter &p) {
    m_display_nb_frames_skipped = p.as_int();
    };
  cb_handle_ = param_subscriber_->add_parameter_callback("display_nb_frames_skipped", cb);

  //////////////////////////////////////////////////////////////////////
  //                        ROS2 SERVICES                             //
  //////////////////////////////////////////////////////////////////////

  // 'quit' service to exit
  std::string quit_service_name = std::string(this->get_name()) + visp_tracker_common::quit_srv_name;
  auto quit_callback = std::bind(&BaseTracker::quit_callback, this, std::placeholders::_1, std::placeholders::_2);
  m_quit_srv = this->create_service<std_srvs::srv::Trigger>(quit_service_name, quit_callback);

  // 'switch_tracking_status' service to turn ON/OFF the tracking
  std::string switch_tracking_name = std::string(this->get_name()) + visp_tracker_common::switch_tracking_srv_name;
  auto switch_tracking_callback = std::bind(&BaseTracker::switch_tracking_status_callback, this, std::placeholders::_1, std::placeholders::_2);
  m_switch_tracking_status_srv = this->create_service<std_srvs::srv::Trigger>(switch_tracking_name, switch_tracking_callback);

  // 'switch_tracking_status' service to turn ON/OFF the tracking
  std::string switch_visual_name = std::string(this->get_name()) + visp_tracker_common::switch_vismode_srv_name;
  auto switch_visual_callback = std::bind(&BaseTracker::switch_visual_status_callback, this, std::placeholders::_1, std::placeholders::_2);
  m_switch_visual_srv = this->create_service<std_srvs::srv::Trigger>(switch_visual_name, switch_visual_callback);

  //////////////////////////////////////////////////////////////////////
  //                        ROS2 PUB/SUB                              //
  //////////////////////////////////////////////////////////////////////

  // ---- Subscribing to the different topics
  auto n = 10;
  auto qos = rclcpp::QoS(rclcpp::KeepLast(n)).best_effort().durability_volatile();

  m_ref_rgb_cam_info_sub = this->create_subscription<sensor_msgs::msg::CameraInfo>(
    m_rgb_camera_topic_name, qos,
    std::bind(&BaseTracker::color_camera_info_callback, this, std::placeholders::_1));
  RCLCPP_INFO(this->get_logger(), "Subscribed to color camera topic %s", m_rgb_camera_topic_name.c_str());

  // NB: We do not subscribe to the RGB image stream because we might want to perform different operations dependeing on the tracker we use

  // ---- Publishing on different topics
  if (does_publish_features) {
    auto qos_features_pub = rclcpp::QoS(rclcpp::KeepLast(5)).best_effort().transient_local();
    std::string features_topic_name = std::string(this->get_name()) + visp_tracker_common::features2D_topic_name;
    m_features_pub = this->create_publisher<visp_tracker_common::msg::NamedFeatureArray>(features_topic_name, qos_features_pub);
  }

  auto qos_infostr_pub = rclcpp::QoS(rclcpp::KeepLast(5)).best_effort().transient_local();
  std::string infostr_topic_name = std::string(this->get_name()) + visp_tracker_common::info_strings_topic_name;
  m_info_strings_pub = this->create_publisher<visp_tracker_common::msg::InfoStrings>(infostr_topic_name, qos_infostr_pub);

  auto qos_poses_pub = rclcpp::QoS(rclcpp::KeepLast(5)).best_effort().transient_local();
  std::string poses_topic_name = std::string(this->get_name()) + visp_tracker_common::poses_topic_name;
  m_poses_pub = this->create_publisher<visp_tracker_common::msg::NamedPoseArray>(poses_topic_name, qos_poses_pub);
}

//////////////////////////////////////////////////////////////////////
//                        INITIALIZATION                            //
//////////////////////////////////////////////////////////////////////

bool BaseTracker::init()
{
  if (std::string(m_ref_rgb_cam_info_sub->get_topic_name()) == s_dumb_topic_name) {
    RCLCPP_ERROR(this->get_logger(), "'rgb_camera_topic_name' parameter was not set, so the color camera subscriber is ill-initialized.");
    return false;
  }

  if (m_rgb_stream_name == s_dumb_topic_name) {
    RCLCPP_ERROR(this->get_logger(), "'rgb_stream_topic_name' parameter was not set, so the color stream subscriber is ill-initialized.");
    return false;
  }

  bool status = this->init_tracker();
  if (!status) {
    return false;
  }

  if (!m_is_headless_mode) {
    m_info_strings.info_strings.push_back(std::string("Nb frames skipped: ") + std::to_string(m_display_nb_frames_skipped));
    m_info_strings.hor_offset_right_border.push_back(s_default_hor_offset);
  }
  this->init_info_strings(); // Call the overrided method of the inheriting class
  return true;
}


//////////////////////////////////////////////////////////////////////
//                        ROS2 SERVICES                             //
//////////////////////////////////////////////////////////////////////

void BaseTracker::quit_callback(const  std::shared_ptr<std_srvs::srv::Trigger::Request> request,
                           std::shared_ptr<std_srvs::srv::Trigger::Response> response)
{
  (void)request;
  stop_and_quit();
  response->success = true;
  response->message = "The node will quit as soon as its last loop ends";
}

void BaseTracker::switch_tracking_status_callback(const  std::shared_ptr<std_srvs::srv::Trigger::Request> request,
                           std::shared_ptr<std_srvs::srv::Trigger::Response> response)
{
  (void)request;
  std::scoped_lock lock(m_mutex_tracking);
  m_has_to_track = (!m_has_to_track);
  response->success = m_has_to_track;
  response->message = "The node will " + (m_has_to_track ? std::string("start") : std::string("stop")) + " tracking as soon as its last loop ends";
}

void BaseTracker::switch_visual_status_callback(const  std::shared_ptr<std_srvs::srv::Trigger::Request> request,
                           std::shared_ptr<std_srvs::srv::Trigger::Response> response)
{
  (void)request;
  std::scoped_lock lock(m_mutex_visualization);
  m_visualization_debug = (!m_visualization_debug);
  response->success = m_visualization_debug;
  response->message = "The node will " + (m_visualization_debug ? std::string("start") : std::string("stop")) + " sending visualization information as soon as its last loop ends";
}

//////////////////////////////////////////////////////////////////////
//                        ROS2 SUBCRIPTIONS                         //
//////////////////////////////////////////////////////////////////////

void BaseTracker::color_camera_info_callback(const sensor_msgs::msg::CameraInfo::SharedPtr msg)
{
  if (m_rgb_cam_info_received) {
    return;
  }

  m_rgb_cam = visp_common::camera::toVispCameraParameters(msg);

  m_rgb_cam_info_received = true;
  m_ref_rgb_cam_info_sub.reset(); // Remove the subscription to avoid unecessary interruptions

  RCLCPP_INFO(this->get_logger(), "RGB camera intrinsics received: fx=%.2f fy=%.2f cx=%.2f cy=%.2f", msg->k[0], msg->k[4], msg->k[2], msg->k[5]);
}

//////////////////////////////////////////////////////////////////////
//                        OTHERS                                    //
//////////////////////////////////////////////////////////////////////

void BaseTracker::stop_and_quit()
{
  std::scoped_lock lock(m_mutex_quit);
  m_quit = true;
}
}
