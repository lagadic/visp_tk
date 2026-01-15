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
    RCLCPP_ERROR(this->get_logger(), "'depth_camera_topic_name' has not been set ! Setting a dumb value.");
    m_depth_camera_topic_name = BaseTracker::s_dumb_topic_name;
  }

  auto depth_stream_topic_name_param = rclcpp::Parameter();
  auto depth_stream_topic_name_desc = rcl_interfaces::msg::ParameterDescriptor {};
  depth_stream_topic_name_desc.description = "Name of the depth image topic.";
  this->declare_parameter("depth_stream_topic_name", "", depth_stream_topic_name_desc);
  this->get_parameter("depth_stream_topic_name", depth_stream_topic_name_param);
  m_depth_stream_name = depth_stream_topic_name_param.as_string();
  if (m_depth_stream_name.empty()) {
    RCLCPP_ERROR(this->get_logger(), "'depth_stream_topic_name' has not been set ! Setting a dumb value.");
    m_depth_stream_name = BaseTracker::s_dumb_topic_name;
  }

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
    std::bind(&BaseMultiModalTracker::color_camera_info_callback, this, std::placeholders::_1));
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

  m_depth_cam = visp_common::toVispCameraParameters(msg);

  m_depth_cam_info_received = true;
  m_depth_cam_info_sub.reset(); // Remove the subscription to avoid unecessary interruptions

  RCLCPP_INFO(this->get_logger(), "Depth camera intrinsics received: fx=%.2f fy=%.2f cx=%.2f cy=%.2f", msg->k[0], msg->k[4], msg->k[2], msg->k[5]);
}

//////////////////////////////////////////////////////////////////////
//                        OTHERS                                    //
//////////////////////////////////////////////////////////////////////


}
