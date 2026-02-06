#include <visp_tracker_common/BaseMultiModalTracker.hpp>

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
    RCLCPP_WARN(this->get_logger(), "'%s' has not been set ! Setting a dumb value.", depth_camera_topic_name_param.get_name().c_str());
    m_depth_camera_topic_name = BaseTracker::s_dumb_topic_name;
  }

  auto depth_stream_topic_name_param = rclcpp::Parameter();
  auto depth_stream_topic_name_desc = rcl_interfaces::msg::ParameterDescriptor {};
  depth_stream_topic_name_desc.description = "Name of the depth image topic.";
  this->declare_parameter("depth_stream_topic_name", "", depth_stream_topic_name_desc);
  this->get_parameter("depth_stream_topic_name", depth_stream_topic_name_param);
  m_depth_stream_name = depth_stream_topic_name_param.as_string();
  if (m_depth_stream_name.empty()) {
    RCLCPP_WARN(this->get_logger(), "'%s' has not been set ! Setting a dumb value.", depth_stream_topic_name_param.get_name().c_str());
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

  auto z_factor_param = rclcpp::Parameter();
  auto z_factor_desc = rcl_interfaces::msg::ParameterDescriptor {};
  z_factor_desc.description = "Factor to convert the depth image expressed as uint16_t into meters.";
  this->declare_parameter("z_factor", 0.001, z_factor_desc);
  this->get_parameter("z_factor", z_factor_param);
  m_z_factor = z_factor_param.as_double();


  // // ---- Parameters changes handling ----

  //////////////////////////////////////////////////////////////////////
  //                        ROS2 SERVICES                             //
  //////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////
  //                        ROS2 PUB/SUB                              //
  //////////////////////////////////////////////////////////////////////

  // ---- Subscribing to the different topics

  // NB: We do not subscribe to the depth image stream nor the depth camera info because we might want to perform different operations depending on the tracker we use

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
    if (m_depth_camera_topic_name == BaseTracker::s_dumb_topic_name) {
      RCLCPP_ERROR(this->get_logger(), "'depth_camera_topic_name' parameter was not set, so the depth camera subscriber is ill-initialized.");
      return false;
    }
    auto n = 10;
    auto qos = rclcpp::QoS(rclcpp::KeepLast(n)).best_effort().durability_volatile();

    m_depth_cam_info_sub = this->create_subscription<sensor_msgs::msg::CameraInfo>(
      m_depth_camera_topic_name, qos,
      std::bind(&BaseMultiModalTracker::depth_camera_info_callback, this, std::placeholders::_1));
    RCLCPP_INFO(this->get_logger(), "Subscribed to depth camera topic %s", m_depth_camera_topic_name.c_str());

    if (m_depth_stream_name == BaseTracker::s_dumb_topic_name) {
      RCLCPP_ERROR(this->get_logger(), "'depth_stream_topic_name' parameter was not set, so the depth stream subscriber is ill-initialized.");
      return false;
    }
    else {
      RCLCPP_INFO(this->get_logger(), "Subscribing to depth topic %s", m_depth_stream_name.c_str());
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
  else {
    rmw_qos_profile_t streams_qos = rmw_qos_profile_default;
    std::string durability_name = this->get_parameter("stream_qos_durability").as_string();
    streams_qos.durability = rmw_qos_durability_policy_from_str(durability_name.c_str());
    std::string reliability_name = this->get_parameter("stream_qos_reliability").as_string();
    streams_qos.reliability = rmw_qos_reliability_policy_from_str(reliability_name.c_str());
    streams_qos.depth = this->get_parameter("stream_qos_depth").as_int();
    rclcpp::QoS qos(streams_qos.depth);
    qos.durability(streams_qos.durability);
    qos.reliability(streams_qos.reliability);
    m_rgb_only_stream_sub = this->create_subscription<sensor_msgs::msg::Image>(m_rgb_stream_name, qos, std::bind(&BaseMultiModalTracker::rgb_callback, this, std::placeholders::_1));
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

void BaseMultiModalTracker::rgb_callback(const sensor_msgs::msg::Image::ConstSharedPtr &rgb)
{
  RCLCPP_DEBUG_STREAM(this->get_logger(), "IN rgb_callback");
  this->treat_rgb(rgb);
  this->track();
  RCLCPP_DEBUG_STREAM(this->get_logger(), "OUT rgb_callback");
}

void BaseMultiModalTracker::sync_callback(const sensor_msgs::msg::Image::ConstSharedPtr &rgb, const sensor_msgs::msg::Image::ConstSharedPtr &depth)
{
  RCLCPP_DEBUG_STREAM(this->get_logger(), "IN sync_callback");
  m_frame_id = rgb->header.frame_id;
  this->treat_rgb(rgb);
  this->treat_depth(depth);
  this->track();
  RCLCPP_DEBUG_STREAM(this->get_logger(), "OUT sync_callback");
}
//////////////////////////////////////////////////////////////////////
//                        OTHERS                                    //
//////////////////////////////////////////////////////////////////////


}
