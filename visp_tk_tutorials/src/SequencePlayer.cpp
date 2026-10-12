#include <visp_tk_tutorials/SequencePlayer.hpp>

namespace visp_tk_tutorials
{

#ifdef ENABLE_VISP_NAMESPACE
using namespace VISP_NAMESPACE_NAME;
#endif

SequencePlayer::SequencePlayer(const std::string &name)
  : rclcpp::Node(name)
{
//////////////////////////////////////////////////////////////////////
  //                        ROS2 PARAMETERS                           //
  //////////////////////////////////////////////////////////////////////
  auto sequence_path_param = rclcpp::Parameter();
  auto sequence_path_desc = rcl_interfaces::msg::ParameterDescriptor { };
  sequence_path_desc.description = "If set, path to the configuration file to initialize the tracker. package:// will be replaced by the path to the share folder of the corresponding package.";
  this->declare_parameter("sequence_path", "", sequence_path_desc);
  this->get_parameter("sequence_path", sequence_path_param);
  m_path = visp_common::path::path_retriever(sequence_path_param.as_string());
  RCLCPP_INFO(this->get_logger(), "m_path = %s", m_path.c_str());
  if ((!sequence_path_param.as_string().empty()) && m_path.empty()) {
    RCLCPP_ERROR(get_logger(), "The package given as %s parameter is not known. Did you source the corresponding workspace?", sequence_path_param.get_name().c_str());
  }

  auto object_name_param = rclcpp::Parameter();
  auto object_name_desc = rcl_interfaces::msg::ParameterDescriptor { };
  object_name_desc.description = "Name of the object that we are interested in.";
  this->declare_parameter("object_name", "", object_name_desc);
  this->get_parameter("object_name", object_name_param);
  m_object_name = object_name_param.as_string();

  // // ---- Parameters related to the services ----

  // // ---- Parameters related to the publishers / subscribers ----
  auto depth_camera_topic_name_desc = rcl_interfaces::msg::ParameterDescriptor { };
  depth_camera_topic_name_desc.description = "Name of the depth camera topic.";
  this->declare_parameter("depth_camera_info_topic_name", "", depth_camera_topic_name_desc);
  this->get_parameter("depth_camera_info_topic_name", m_depth_camera_info_topic_name);

  auto depth_image_topic_name_desc = rcl_interfaces::msg::ParameterDescriptor { };
  depth_image_topic_name_desc.description = "Name of the depth image topic.";
  this->declare_parameter("depth_image_topic_name", "", depth_image_topic_name_desc);
  this->get_parameter("depth_image_topic_name", m_depth_stream_name);

  auto rgb_camera_topic_name_desc = rcl_interfaces::msg::ParameterDescriptor { };
  rgb_camera_topic_name_desc.description = "Name of the color camera topic.";
  this->declare_parameter("rgb_camera_info_topic_name", "", rgb_camera_topic_name_desc);
  this->get_parameter("rgb_camera_info_topic_name", m_rgb_camera_info_topic_name);

  auto rgb_image_topic_name_desc = rcl_interfaces::msg::ParameterDescriptor { };
  rgb_image_topic_name_desc.description = "Name of the color image topic.";
  this->declare_parameter("rgb_image_topic_name", "", rgb_image_topic_name_desc);
  this->get_parameter("rgb_image_topic_name", m_rgb_stream_name);

  auto init_pose_topic_name_desc = rcl_interfaces::msg::ParameterDescriptor { };
  init_pose_topic_name_desc.description = "Name of the color camera topic.";
  this->declare_parameter("init_pose_topic_name", "", init_pose_topic_name_desc);
  this->get_parameter("init_pose_topic_name", m_init_pose_topic_name);

  // // ---- Parameters changes handling ----

  //////////////////////////////////////////////////////////////////////
  //                        ROS2 SERVICES                             //
  //////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////
  //                        ROS2 PUB/SUB                              //
  //////////////////////////////////////////////////////////////////////

  // ---- Subscribing to the different topics


  // ---- Publishing on different topics
  // NB: We do not create the publisher of the image streams and camera info because we might want to check that everything is working fine first

  //////////////////////////////////////////////////////////////////////
  //                        ROS2 TIMERS                               //
  //////////////////////////////////////////////////////////////////////
  // Adding a periodic callback for the servoing
  this->m_timer = this->create_wall_timer(
    std::chrono::milliseconds(30),
    std::bind(&SequencePlayer::periodic_publication, this));

}

bool SequencePlayer::init()
{
  if (!loadCameraSettings()) {
    return false;
  }
  if (!loadFrames()) {
    return false;
  }

  if (!getInitialPoses()) {
    return false;
  }

  if (m_depth_camera_info_topic_name.as_string().empty()) {
    RCLCPP_ERROR(this->get_logger(), "'%s' has not been set !", m_depth_camera_info_topic_name.get_name().c_str());
    return false;
  }
  if (m_depth_stream_name.as_string().empty()) {
    RCLCPP_ERROR(this->get_logger(), "'%s' has not been set !", m_depth_stream_name.get_name().c_str());
    return false;
  }
  if (m_rgb_camera_info_topic_name.as_string().empty()) {
    RCLCPP_ERROR(this->get_logger(), "'%s' has not been set !", m_rgb_camera_info_topic_name.get_name().c_str());
    return false;
  }
  if (m_rgb_stream_name.as_string().empty()) {
    RCLCPP_ERROR(this->get_logger(), "'%s' has not been set !", m_rgb_stream_name.get_name().c_str());
    return false;
  }
  if (m_init_pose_topic_name.as_string().empty()) {
    RCLCPP_ERROR(this->get_logger(), "'%s' has not been set !", m_init_pose_topic_name.get_name().c_str());
    return false;
  }

  auto qos_poses_pub = rclcpp::QoS(rclcpp::KeepLast(5)).best_effort().transient_local();
  m_init_pose_pub = this->create_publisher<geometry_msgs::msg::PoseStamped>(m_init_pose_topic_name.as_string(), qos_poses_pub);

  m_depth_cam_info_pub = this->create_publisher<sensor_msgs::msg::CameraInfo>(m_depth_camera_info_topic_name.as_string(), qos_poses_pub);
  m_depth_img_pub = this->create_publisher<sensor_msgs::msg::Image>(m_depth_stream_name.as_string(), qos_poses_pub);
  m_rgb_cam_info_pub = this->create_publisher<sensor_msgs::msg::CameraInfo>(m_rgb_camera_info_topic_name.as_string(), qos_poses_pub);
  m_rgb_img_pub = this->create_publisher<sensor_msgs::msg::Image>(m_rgb_stream_name.as_string(), qos_poses_pub);
  return true;
}

bool SequencePlayer::loadCameraSettings()
{
  const std::string camFile = vpIoTools::createFilePath(m_path, "cam.json");
  if (!vpIoTools::checkFilename(camFile)) {
    RCLCPP_ERROR(this->get_logger(), "Camera file %s does not exist", camFile.c_str());
    return false;
  }
  std::ifstream cf(camFile);

  if (!cf.good()) {
    RCLCPP_ERROR(this->get_logger(), "Problem opening %s", camFile.c_str());
    return false;
  }
  const nlohmann::json j = nlohmann::json::parse(cf);
  m_cam.initPersProjWithoutDistortion(j.at("px"), j.at("py"), j.at("u0"), j.at("v0"));
  m_h = j.at("h"), m_w = j.at("w");
  m_depthScale = j.at("depthScale");

  m_cam_info = visp_common::camera::toSensorMsgsCameraInfo(m_cam, m_w, m_h);
  return true;
}

const std::string SequencePlayer::getColorFrame(unsigned int index)
{
  std::stringstream colorName;
  colorName << "color_image_" << std::setfill('0') << std::setw(4) << index << ".png";
  return vpIoTools::createFilePath(m_path, colorName.str());
}

const std::string SequencePlayer::getDepthFrame(unsigned int index)
{
  std::stringstream colorName;
  colorName << "depth_image_" << std::setfill('0') << std::setw(4) << index << ".npz";
  return vpIoTools::createFilePath(m_path, colorName.str());
}

bool SequencePlayer::loadFrames()
{
  unsigned int frameIndex = 0;
  while (true) {
    SequenceFrame frame;
    const std::string colorFramePath = getColorFrame(frameIndex);
    if (!vpIoTools::checkFilename(colorFramePath)) {
      break;
    }
    vpImage<vpRGBa> IRGB;
    vpImageIo::read(IRGB, colorFramePath);
    frame.IRGB = visp_common::image::toSensorMsgsImage(IRGB);

    const std::string depthFramePath = getDepthFrame(frameIndex);
    if (vpIoTools::checkFilename(depthFramePath)) {
      visp::cnpy::NpyArray depth_data = visp::cnpy::npz_load(depthFramePath).find("data")->second;
      vpImage<uint16_t> depthRaw(depth_data.data<uint16_t>(), m_h, m_w, true);
      frame.depth = visp_common::image::toSensorMsgsImage(depthRaw);
    }

    m_frames.push_back(frame);
    ++frameIndex;
  }
  if (frameIndex == 0) {
    RCLCPP_ERROR(this->get_logger(), "Could not read any frames from the folder %s", m_path.c_str());
    return false;
  }
  else {
    return true;
  }
}

bool SequencePlayer::getInitialPoses()
{
  std::string initsFolder = vpIoTools::createFilePath(m_path, "init");
  const std::string objectInitFile = vpIoTools::createFilePath(initsFolder, m_object_name + ".json");

  if (vpIoTools::checkFilename(objectInitFile)) {
    std::ifstream f(objectInitFile);
    if (f.good()) {
      m_initial_cMo = nlohmann::json::parse(f);
    }
    else {
      RCLCPP_ERROR(this->get_logger(), "There was an issue opening init file %s", objectInitFile.c_str());
      return false;
    }
    f.close();
  }
  else {
    RCLCPP_ERROR(this->get_logger(), "The init file %s does not exist.", objectInitFile.c_str());
    return false;
  }
  m_initial_pose.pose = visp_common::pose::toGeometryMsgsPose(m_initial_cMo);
  return true;
}

void SequencePlayer::periodic_publication()
{
  static unsigned int frame_id = 0;
  static unsigned char frame_0_iter = 0;
  const unsigned char NB_REPET_FRAME_0 = 150; // The first frame will be repeated during around 5s
  // Updating headers
  SequenceFrame &frame = getFrame(frame_id);
  if (frame_id == 0) {
    // Repeating first frame to let the user the time to click
    ++frame_0_iter;
    if (frame_0_iter == NB_REPET_FRAME_0) {
      frame_0_iter = 0;
      ++frame_id;
    }
  }
  else {
    frame_id = (frame_id + 1) % numFrames();
  }
  std_msgs::msg::Header header;
  header.stamp = this->now();
  header.frame_id = "rgbd_camera";
  m_cam_info.header = header;
  frame.depth.header = header;
  frame.IRGB.header = header;
  m_initial_pose.header = header;

  // Publishing data
  m_depth_cam_info_pub->publish(m_cam_info);
  m_depth_img_pub->publish(frame.depth);
  m_rgb_cam_info_pub->publish(m_cam_info);
  m_rgb_img_pub->publish(frame.IRGB);
  m_init_pose_pub->publish(m_initial_pose);
}
}
