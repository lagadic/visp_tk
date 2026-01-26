#include <visp_tracker_common/BaseMultiModalTracker.hpp>

#include <visp_common/qos.hpp>

namespace visp_tracker_common
{
BaseMultiModalTracker::BaseMultiModalTracker(const std::string &name, const bool &does_publish_features) : BaseTracker(name, does_publish_features)
{
  //////////////////////////////////////////////////////////////////////
  //                        ROS2 PARAMETERS                           //
  //////////////////////////////////////////////////////////////////////

  // // ---- Parameters related to the services ----

  // // ---- Parameters related to the publishers / subscribers ----
  auto depth_camera_topic_name_param = rclcpp::Parameter();
  auto depth_camera_topic_name_desc = rcl_interfaces::msg::ParameterDescriptor {};
  depth_camera_topic_name_desc.description = "Name of the depth camera topic.";
  this->declare_parameter("depth_camera_topic_name", "", depth_camera_topic_name_desc);
  this->get_parameter("depth_camera_topic_name", depth_camera_topic_name_param);
  m_depth_camera_topic_name = depth_camera_topic_name_param.as_string();
  if (m_depth_camera_topic_name.empty()) {
    RCLCPP_ERROR(this->get_logger(), "'%s' has not been set ! Setting a dumb value.", depth_camera_topic_name_param.get_name().c_str());
    m_depth_camera_topic_name = BaseTracker::s_dumb_topic_name;
  }

  auto depth_stream_topic_name_param = rclcpp::Parameter();
  auto depth_stream_topic_name_desc = rcl_interfaces::msg::ParameterDescriptor {};
  depth_stream_topic_name_desc.description = "Name of the depth image topic.";
  this->declare_parameter("depth_stream_topic_name", "", depth_stream_topic_name_desc);
  this->get_parameter("depth_stream_topic_name", depth_stream_topic_name_param);
  m_depth_stream_name = depth_stream_topic_name_param.as_string();
  if (m_depth_stream_name.empty()) {
    RCLCPP_ERROR(this->get_logger(), "'%s' has not been set ! Setting a dumb value.", depth_stream_topic_name_param.get_name().c_str());
    m_depth_stream_name = BaseTracker::s_dumb_topic_name;
  }

  auto durability_desc = rcl_interfaces::msg::ParameterDescriptor {};
  durability_desc.description = "The durability of both the RGB and depth image streams (they need to be the same) if depth is required.";
  this->declare_parameter("stream_qos_durability", "volatile", durability_desc);

  auto reliability_desc = rcl_interfaces::msg::ParameterDescriptor {};
  reliability_desc.description = "The reliability of both the RGB and depth image streams (they need to be the same) if depth is required.";
  this->declare_parameter("stream_qos_reliability", "best_effort", reliability_desc);

  auto qos_depth_desc = rcl_interfaces::msg::ParameterDescriptor {};
  qos_depth_desc.description = "The depth of the queue of both the RGB and depth image streams (they need to be the same) if depth is required.";
  this->declare_parameter("stream_qos_depth", 1, qos_depth_desc);

  // // ---- Parameters changes handling ----

  //////////////////////////////////////////////////////////////////////
  //                        ROS2 SERVICES                             //
  //////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////
  //                        ROS2 PUB/SUB                              //
  //////////////////////////////////////////////////////////////////////

  // ---- Subscribing to the different topics
  auto n = 10;
  auto qos = rclcpp::QoS(rclcpp::KeepLast(n)).best_effort().durability_volatile();

  m_depth_cam_info_sub = this->create_subscription<sensor_msgs::msg::CameraInfo>(
    m_depth_camera_topic_name, qos,
    std::bind(&BaseMultiModalTracker::depth_camera_info_callback, this, std::placeholders::_1));
  RCLCPP_INFO(this->get_logger(), "Subscribed to depth camera topic %s", m_depth_camera_topic_name.c_str());

  // NB: We do not subscribe to the depth image stream because we might want to perform different operations dependeing on the tracker we use

  // ---- Publishing on different topics

}

//////////////////////////////////////////////////////////////////////
//                        INITIALIZATION                            //
//////////////////////////////////////////////////////////////////////

bool BaseMultiModalTracker::init()
{
  bool status = BaseTracker::init();
  if (!status) {
    return status;
  }

  check_requires_depth();

  if (m_depth_is_required) {
    if (std::string(m_depth_cam_info_sub->get_topic_name()) == BaseTracker::s_dumb_topic_name) {
      RCLCPP_ERROR(this->get_logger(), "'depth_camera_topic_name' parameter was not set, so the depth camera subscriber is ill-initialized.");
      return false;
    }

    if (m_depth_stream_name == BaseTracker::s_dumb_topic_name) {
      RCLCPP_ERROR(this->get_logger(), "'depth_stream_topic_name' parameter was not set, so the depth stream subscriber is ill-initialized.");
      return false;
    }

    rmw_qos_profile_t streams_qos = rmw_qos_profile_default;
    std::string durability_name = this->get_parameter("stream_qos_durability").as_string();
    streams_qos.durability = rmw_qos_durability_policy_from_str(durability_name.c_str());
    std::string reliability_name = this->get_parameter("stream_qos_reliability").as_string();
    streams_qos.reliability = rmw_qos_reliability_policy_from_str(reliability_name.c_str());
    streams_qos.depth = this->get_parameter("stream_qos_depth").as_int();

    m_rgb_stream_sub.subscribe(this, m_rgb_stream_name, streams_qos);
    m_depth_stream_sub.subscribe(this, m_depth_stream_name, streams_qos);

    uint32_t queue_size = 10;
    m_streams_sync = std::make_shared<message_filters::Synchronizer<message_filters::sync_policies::
      ApproximateTime<sensor_msgs::msg::Image, sensor_msgs::msg::Image>>>(
      message_filters::sync_policies::ApproximateTime<sensor_msgs::msg::Image,
      sensor_msgs::msg::Image>(queue_size), m_rgb_stream_sub, m_depth_stream_sub);

    m_streams_sync->setMaxIntervalDuration(rclcpp::Duration(0, 5000000)); // Allow maximum 5ms of difference between two messages
    m_streams_sync->registerCallback(std::bind(&BaseMultiModalTracker::sync_callback, this, std::placeholders::_1, std::placeholders::_2));
  }

  if (!m_is_headless_mode) {
    m_info_strings.info_strings.push_back(std::string("Requires depth : ") + (m_depth_is_required ? std::string("true") : std::string("false")));
    m_info_strings.hor_offset_right_border.push_back(s_default_hor_offset);
  }

  return true;
}


//////////////////////////////////////////////////////////////////////
//                        ROS2 SERVICES                             //
//////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////
//                        ROS2 SUBCRIPTIONS                         //
//////////////////////////////////////////////////////////////////////

void BaseMultiModalTracker::depth_camera_info_callback(const sensor_msgs::msg::CameraInfo::SharedPtr msg)
{
  if (m_depth_cam_info_received) {
    return;
  }

  m_depth_cam = visp_common::camera::toVispCameraParameters(msg);

  m_depth_cam_info_received = true;
  m_depth_cam_info_sub.reset(); // Remove the subscription to avoid unecessary interruptions

  RCLCPP_INFO(this->get_logger(), "Depth camera intrinsics received: fx=%.2f fy=%.2f cx=%.2f cy=%.2f", msg->k[0], msg->k[4], msg->k[2], msg->k[5]);
}


void BaseMultiModalTracker::sync_callback(const sensor_msgs::msg::Image::ConstSharedPtr &rgb, const sensor_msgs::msg::Image::ConstSharedPtr &depth)
{
  RCLCPP_INFO_STREAM(this->get_logger(), "IN sync_callback");
  this->treat_rgb(rgb);
  this->treat_depth(depth);
  this->track();
  RCLCPP_INFO_STREAM(this->get_logger(), "OUT sync_callback");
}
//////////////////////////////////////////////////////////////////////
//                        OTHERS                                    //
//////////////////////////////////////////////////////////////////////


}
