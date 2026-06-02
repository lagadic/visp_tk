/*
 * Copyright (C) 2026 by Inria. All rights reserved.
 *
 * This software is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * See the file LICENSE.txt at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * For using this software that can not be combined with the GNU
 * GPL, or if you have questions regarding the use of this file,
 * please contact Inria at visp@inria.fr
 *
 * This software was developed at:
 * Inria centre at Rennes University
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <visp_apriltag/AprilTagTracker.hpp>


#if VISP_VERSION_INT <= VP_VERSION_INT(3, 7, 0)
namespace
{
vpDetectorAprilTag::vpAprilTagFamily tagFamilyFromString(const std::string &name)
{
  if (vpIoTools::toLowerCase(name) == "36h10") {
    return vpDetectorAprilTag::TAG_36h10;
  }
  else if (vpIoTools::toLowerCase(name) == "36h11") {
    return vpDetectorAprilTag::TAG_36h11;
  }
  else if (vpIoTools::toLowerCase(name) == "25h9") {
    return vpDetectorAprilTag::TAG_25h9;
  }
  else if (vpIoTools::toLowerCase(name) == "25h7") {
    return vpDetectorAprilTag::TAG_25h7;
  }
  else if (vpIoTools::toLowerCase(name) == "16h5") {
    return vpDetectorAprilTag::TAG_16h5;
  }
  else if (vpIoTools::toLowerCase(name) == "circle21h7") {
    return vpDetectorAprilTag::TAG_CIRCLE21h7;
  }
  else if (vpIoTools::toLowerCase(name) == "circle49h12") {
    return vpDetectorAprilTag::TAG_CIRCLE49h12;
  }
  else if (vpIoTools::toLowerCase(name) == "custom48h12") {
    return vpDetectorAprilTag::TAG_CUSTOM48h12;
  }
  else if (vpIoTools::toLowerCase(name) == "standard41h12") {
    return vpDetectorAprilTag::TAG_STANDARD41h12;
  }
  else if (vpIoTools::toLowerCase(name) == "standard41h12") {
    return vpDetectorAprilTag::TAG_STANDARD52h13;
  }
#if defined(VISP_HAVE_APRILTAG_ARUCO)
  else if (vpIoTools::toLowerCase(name) == "aruco_4x4_50") {
    return vpDetectorAprilTag::TAG_ARUCO_4x4_50;
  }
  else if (vpIoTools::toLowerCase(name) == "aruco_4x4_100") {
    return vpDetectorAprilTag::TAG_ARUCO_4x4_100;
  }
  else if (vpIoTools::toLowerCase(name) == "aruco_4x4_250") {
    return vpDetectorAprilTag::TAG_ARUCO_4x4_250;
  }
  else if (vpIoTools::toLowerCase(name) == "aruco_4x4_1000") {
    return vpDetectorAprilTag::TAG_ARUCO_4x4_1000;
  }
  else if (vpIoTools::toLowerCase(name) == "aruco_5x5_50") {
    return vpDetectorAprilTag::TAG_ARUCO_5x5_50;
  }
  else if (vpIoTools::toLowerCase(name) == "aruco_5x5_100") {
    return vpDetectorAprilTag::TAG_ARUCO_5x5_100;
  }
  else if (vpIoTools::toLowerCase(name) == "aruco_5x5_250") {
    return vpDetectorAprilTag::TAG_ARUCO_5x5_250;
  }
  else if (vpIoTools::toLowerCase(name) == "aruco_5x5_1000") {
    return vpDetectorAprilTag::TAG_ARUCO_5x5_1000;
  }
  else if (vpIoTools::toLowerCase(name) == "aruco_6x6_50") {
    return vpDetectorAprilTag::TAG_ARUCO_6x6_50;
  }
  else if (vpIoTools::toLowerCase(name) == "aruco_6x6_100") {
    return vpDetectorAprilTag::TAG_ARUCO_6x6_100;
  }
  else if (vpIoTools::toLowerCase(name) == "aruco_6x6_250") {
    return vpDetectorAprilTag::TAG_ARUCO_6x6_250;
  }
  else if (vpIoTools::toLowerCase(name) == "aruco_6x6_1000") {
    return vpDetectorAprilTag::TAG_ARUCO_6x6_1000;
  }
  else if (vpIoTools::toLowerCase(name) == "aruco_7x7_50") {
    return vpDetectorAprilTag::TAG_ARUCO_7x7_50;
  }
  else if (vpIoTools::toLowerCase(name) == "aruco_7x7_100") {
    return vpDetectorAprilTag::TAG_ARUCO_7x7_100;
  }
  else if (vpIoTools::toLowerCase(name) == "aruco_7x7_250") {
    return vpDetectorAprilTag::TAG_ARUCO_7x7_250;
  }
  else if (vpIoTools::toLowerCase(name) == "aruco_7x7_1000") {
    return vpDetectorAprilTag::TAG_ARUCO_7x7_1000;
  }
  else if (vpIoTools::toLowerCase(name) == "aruco_mip_36h12") {
    return vpDetectorAprilTag::TAG_ARUCO_MIP_36h12;
  }
#endif
  throw(vpException(vpException::badValue, "Could not find a tag family that corresponds to the name '%s'", name.c_str()));
}

vpDetectorAprilTag::vpPoseEstimationMethod poseMethodFromString(const std::string &name)
{
  if (vpIoTools::toLowerCase(name) == "dementhon_virtual_vs") {
    return vpDetectorAprilTag::DEMENTHON_VIRTUAL_VS;
  }
  else if (vpIoTools::toLowerCase(name) == "lagrange_virtual_vs") {
    return vpDetectorAprilTag::LAGRANGE_VIRTUAL_VS;
  }
  else if (vpIoTools::toLowerCase(name) == "best_residual_virtual_vs") {
    return vpDetectorAprilTag::BEST_RESIDUAL_VIRTUAL_VS;
  }
#if defined(VISP_HAVE_APRILTAG_POSE_FCT)
  else if (vpIoTools::toLowerCase(name) == "homography") {
    return vpDetectorAprilTag::HOMOGRAPHY;
  }
  else if (vpIoTools::toLowerCase(name) == "homography_virtual_vs") {
    return vpDetectorAprilTag::HOMOGRAPHY_VIRTUAL_VS;
  }
  else if (vpIoTools::toLowerCase(name) == "homography_orthogonal_iteration") {
    return vpDetectorAprilTag::HOMOGRAPHY_ORTHOGONAL_ITERATION;
  }
#endif
  throw(vpException(vpException::badValue, "Could not find a pose method that corresponds to the name '%s'", name.c_str()));
}

}
#endif

namespace visp_apriltag
{
void fromImagePoint(const vpImagePoint &ip, vision_msgs::msg::Point2D &out)
{
  out.x = ip.get_u();
  out.y = ip.get_v();
}

AprilTagTracker::AprilTagTracker(const std::string &node_name)
  : visp_tracker_common::BaseTracker(node_name, true)
{
  //////////////////////////////////////////////////////////////////////
  //                        ROS2 PARAMETERS                           //
  //////////////////////////////////////////////////////////////////////
  auto tag_size_value_param_desc = rcl_interfaces::msg::ParameterDescriptor {};
  tag_size_value_param_desc.description = "This parameter indicates the size of the tag, expressed in meters. See https://visp-doc.inria.fr/doxygen/visp-daily/classvpDetectorAprilTag.html for more information";
  this->declare_parameter<std::vector<double>>("tag_size_values", std::vector<double>(), tag_size_value_param_desc);

  auto tag_size_key_param_desc = rcl_interfaces::msg::ParameterDescriptor {};
  tag_size_key_param_desc.description = "This parameter indicates the ID associated to the size of the tag. ID -1 is a special case that means 'any ID that is not explicitly listed in the map'";
  this->declare_parameter<std::vector<int64_t>>("tag_size_keys", std::vector<int64_t>(), tag_size_key_param_desc);

  auto id_pub_param_desc = rcl_interfaces::msg::ParameterDescriptor {};
  id_pub_param_desc.description = "If set, the ID of the tag whose pose must be published on the pose topic.";
  this->declare_parameter<int>("id_published", -1, id_pub_param_desc);

  auto tag_family_param_desc = rcl_interfaces::msg::ParameterDescriptor {};
#if VISP_VERSION_INT > VP_VERSION_INT(3, 7, 0)
  tag_family_param_desc.description = "This parameter indicates the family of the tag. Available families are " + vpDetectorAprilTag::getAvailableTagFamily();
#else
  tag_family_param_desc.description = "This parameter indicates the family of the tag.";
#endif
  this->declare_parameter<std::string>("tag_family", "", tag_family_param_desc);

  auto pose_method_param_desc = rcl_interfaces::msg::ParameterDescriptor {};
#if VISP_VERSION_INT > VP_VERSION_INT(3, 7, 0)
  pose_method_param_desc.description = "This parameter indicates the pose estimation to use when a tag is detected. Available methods are " + vpDetectorAprilTag::getAvailablePoseMethod();
#else
  pose_method_param_desc.description = "This parameter indicates the pose estimation to use when a tag is detected.";
#endif
  this->declare_parameter<std::string>("pose_method", "", pose_method_param_desc);

  auto detection_margin_param_desc = rcl_interfaces::msg::ParameterDescriptor {};
  detection_margin_param_desc.description = "This parameter indicates the detection margin threshold. Setting -1 deactivate the feature. See https://visp-doc.inria.fr/doxygen/visp-daily/classvpDetectorAprilTag.html for more information";
  this->declare_parameter<double>("detection_margin_thresh", -1.0, detection_margin_param_desc);

  auto align_z_param_desc = rcl_interfaces::msg::ParameterDescriptor {};
  align_z_param_desc.description = "This parameter indicates if the Z axis of the tag must be aligned with the one of the camera. See https://visp-doc.inria.fr/doxygen/visp-daily/classvpDetectorAprilTag.html for more information";
  this->declare_parameter<bool>("align_z", false, align_z_param_desc);

  auto display_tag_param_desc = rcl_interfaces::msg::ParameterDescriptor {};
  display_tag_param_desc.description = "This parameter indicates if the detector must display the tag on screen. See https://visp-doc.inria.fr/doxygen/visp-daily/classvpDetectorAprilTag.html for more information";
  this->declare_parameter<bool>("display_tag", false, display_tag_param_desc);

  //////////////////////////////////////////////////////////////////////
  //                        ROS2 PUB/SUB                              //
  //////////////////////////////////////////////////////////////////////

  // ---- Subscribing to the different topics
  auto n = 10;
  auto qos = rclcpp::QoS(rclcpp::KeepLast(n)).best_effort().durability_volatile();
  m_rgb_stream_sub = this->create_subscription<sensor_msgs::msg::Image>(
    m_rgb_stream_name, qos,
    std::bind(&AprilTagTracker::image_callback, this, std::placeholders::_1));
  RCLCPP_INFO(this->get_logger(), "Subscribed to image topic '%s'", m_rgb_stream_name.c_str());

  // ---- Publishing on different topics
  auto qos_tag_info_pub = rclcpp::QoS(rclcpp::KeepLast(5)).best_effort().transient_local();
  std::string tag_info_name = std::string(this->get_name()) + "/tags_info";
  m_tags_info_pub = this->create_publisher<visp_tracker_common::msg::AprilTagDetectionArray>(tag_info_name, qos_tag_info_pub);
}

//////////////////////////////////////////////////////////////////////
//                        INITIALIZATION                            //
//////////////////////////////////////////////////////////////////////

bool AprilTagTracker::init_tracker()
{
  RCLCPP_INFO(this->get_logger(), "This node does not require an initialization method");
  auto id_pub_param = rclcpp::Parameter();
  this->get_parameter("id_published", id_pub_param);
  m_opt_id = (id_pub_param.as_int() >= 0 ? std::optional<int>(id_pub_param.as_int()) : std::nullopt);
  if (m_opt_id) {
    RCLCPP_INFO(this->get_logger(), "The pose of the tag ID %d will be published on the topic %s.", *m_opt_id, m_poses_pub->get_topic_name());
  }

  std::vector<double> tag_size_values = this->get_parameter("tag_size_values").as_double_array();
  if (tag_size_values.size() == 0) {
    RCLCPP_ERROR(this->get_logger(), "'tag_size_values' parameter is not set");
    return false;
  }

  std::vector<int64_t> tag_size_keys = this->get_parameter("tag_size_keys").as_integer_array();
  if (tag_size_keys.size() == 0) {
    RCLCPP_ERROR(this->get_logger(), "'tag_size_keys' parameter is not set");
    return false;
  }

  if (tag_size_keys.size() != tag_size_values.size()) {
    RCLCPP_ERROR_STREAM(this->get_logger(), "'tag_size_keys' size(" << tag_size_keys.size()<< ") differs from 'tag_size_values' size (" << tag_size_values.size() << ")");
    return false;
  }

  size_t nb_keys = tag_size_keys.size();
  for (size_t i = 0; i < nb_keys; ++i) {
    int64_t key = tag_size_keys[i];
    float val = static_cast<float>(tag_size_values[i]);
    m_tag_size_map[key] = static_cast<float>(val);
  }
  auto idx_tag_size = m_tag_size_map.find(-1);
  if (idx_tag_size != m_tag_size_map.end()) {
    m_default_tag_size = idx_tag_size->second;
  }
  else {
    RCLCPP_ERROR_STREAM(this->get_logger(), "'tag_size_keys' does not contain an entry for the wildcard value '-1'");
    return false;
  }

  if (m_config_file.empty()) {
    m_family_name = this->get_parameter("tag_family").as_string();
    if (!m_family_name.empty()) {
      RCLCPP_INFO(this->get_logger(), "Tag family is set to %s\n", m_family_name.c_str());
    }
    else {
      RCLCPP_ERROR(this->get_logger(), "'tag_family' parameter is not set");
      return false;
    }

    try {
#if VISP_VERSION_INT > VP_VERSION_INT(3, 7, 0)
      vpDetectorAprilTag::vpAprilTagFamily tag_family = vpDetectorAprilTag::tagFamilyFromString(m_family_name);
#else
      vpDetectorAprilTag::vpAprilTagFamily tag_family = tagFamilyFromString(m_family_name);
#endif
      m_tag_detector.setAprilTagFamily(tag_family);
    }
    catch (const vpException &e) {
#if VISP_VERSION_INT > VP_VERSION_INT(3, 7, 0)
      RCLCPP_ERROR(this->get_logger(), "'tag_family' parameter value '%s' cannot be converted to a known family. Allowed values are: %s", m_family_name.c_str(), vpDetectorAprilTag::getAvailableTagFamily().c_str());
#else
      RCLCPP_ERROR(this->get_logger(), "'tag_family' parameter value '%s' cannot be converted to a known family.", m_family_name.c_str());
#endif
      return false;
    }

    auto pose_method_name = this->get_parameter("pose_method").as_string();
    if (pose_method_name.empty()) {
      RCLCPP_ERROR(this->get_logger(), "'pose_method' parameter is not set");
      return false;
    }
    try {
#if VISP_VERSION_INT > VP_VERSION_INT(3, 7, 0)
      vpDetectorAprilTag::vpPoseEstimationMethod pose_method = vpDetectorAprilTag::poseMethodFromString(pose_method_name);
#else
      vpDetectorAprilTag::vpPoseEstimationMethod pose_method = poseMethodFromString(pose_method_name);
#endif
      m_tag_detector.setAprilTagPoseEstimationMethod(pose_method);
      RCLCPP_INFO(this->get_logger(), "Pose estimation method is set to %s\n", pose_method_name.c_str());
    }
    catch (const vpException &e) {
#if VISP_VERSION_INT > VP_VERSION_INT(3, 7, 0)
      RCLCPP_ERROR(this->get_logger(), "'pose_method' parameter value '%s' cannot be converted to a known family. Allowed values are: %s", pose_method_name.c_str(), vpDetectorAprilTag::getAvailablePoseMethod().c_str());
#else
      RCLCPP_ERROR(this->get_logger(), "'pose_method' parameter value '%s' cannot be converted to a known family.", pose_method_name.c_str());
#endif
      return false;
    }

    auto margin_thresh_param = rclcpp::Parameter();
    this->get_parameter("detection_margin_thresh", margin_thresh_param);
    auto margin_thresh = margin_thresh_param.as_double();
    m_tag_detector.setAprilTagDecisionMarginThreshold(margin_thresh);

    auto display_tag_param = rclcpp::Parameter();
    this->get_parameter("display_tag", display_tag_param);
    auto display_tag = display_tag_param.as_bool();
    m_tag_detector.setDisplayTag(display_tag);

    auto align_z_param = rclcpp::Parameter();
    this->get_parameter("align_z", align_z_param);
    auto align_z = align_z_param.as_bool();
    m_tag_detector.setZAlignedWithCameraAxis(align_z);
    RCLCPP_INFO(this->get_logger(), "Tracker settings:");
    RCLCPP_INFO_STREAM(this->get_logger(), "  Tag family: " << m_family_name);
    RCLCPP_INFO_STREAM(this->get_logger(), "  Z aligned: " << (align_z ? "yes" : "no"));
    RCLCPP_INFO_STREAM(this->get_logger(), "  Display: " << (display_tag ? "yes" : "no"));

    RCLCPP_INFO(this->get_logger(), "Tracker initialization Done !");
    return true;
  }
  else {
#if VISP_VERSION_INT > VP_VERSION_INT(3, 7, 0)
    try {
      RCLCPP_INFO(this->get_logger(), "Reading configuration file %s\n", m_config_file.c_str());
      m_tag_detector.loadConfigFile(m_config_file);
      m_family_name = vpDetectorAprilTag::tagFamilyToString(m_tag_detector.getAprilTagFamily());
    }
    catch (const vpException &e) {
      RCLCPP_ERROR(this->get_logger(), e.what());
      return false;
    }
    return true;
#else
    throw(vpException(vpException::badValue, "Could not initialize apriltag tracker from config file."
                      "Feature supported only since ViSP 3.7.1"));
#endif
  }
}



void AprilTagTracker::init_info_strings()
{
  const unsigned int nb_digits = 3;
  const int nb_info = m_info_strings.hor_offset_right_border.size();
  // Correct the horizontal offset to match the one required for the
  for (int idx = 0; idx < nb_info; ++idx) {
    m_info_strings.hor_offset_right_border[idx] = s_hor_offset_from_left_border;
  }
  m_info_strings.info_strings.push_back(std::string("Default Tag Size.: ") + std::to_string(m_default_tag_size).substr(0, std::to_string(m_default_tag_size).find(".") + nb_digits + 1));
  m_info_strings.hor_offset_right_border.push_back(s_hor_offset_from_left_border);
  m_info_strings.info_strings.push_back(std::string("Tag family.......: ") + m_family_name);
  m_info_strings.hor_offset_right_border.push_back(s_hor_offset_from_left_border);
  m_info_strings.info_strings.push_back(std::string("Margin threshold.: ") + (m_tag_detector.getAprilTagDecisionMarginThreshold() < 0 ? std::string("deactivated") : std::to_string(m_tag_detector.getAprilTagDecisionMarginThreshold())));
  m_info_strings.hor_offset_right_border.push_back(s_hor_offset_from_left_border);
  m_info_nb_static = m_info_strings.info_strings.size();
}

//////////////////////////////////////////////////////////////////////
//                        ROS2 SUBCRIPTIONS                         //
//////////////////////////////////////////////////////////////////////

void AprilTagTracker::image_callback(const sensor_msgs::msg::Image::ConstSharedPtr &msg)
{
  RCLCPP_INFO(this->get_logger(), "Receive image");
  try {
    bool quit = false;
    {
      std::scoped_lock lock(m_mutex_quit);
      quit = m_quit;
    }
    bool has_to_track = false;
    {
      std::scoped_lock lock(m_mutex_tracking);
      has_to_track = m_has_to_track;
    }
    if (quit || (!m_rgb_cam_info_received) || ((!has_to_track) && m_is_headless_mode)) {
      if (!m_rgb_cam_info_received) {
        RCLCPP_INFO(this->get_logger(), "Waiting for camera info...");
      }
      return;
    }

    // Check if frame has to be displayed
#if defined(VISP_HAVE_DISPLAY) && defined(VISP_HAVE_MODULE_GUI)
    bool display_frame = (!m_is_headless_mode) &&((m_display_nb_frames_skipped <= 0) || ((m_frame_cnt % m_display_nb_frames_skipped) == 0));
#endif

    // Convert ROS image to ViSP image
    m_I = std::move(visp_common::image::toVispImageChar(*msg));

#if defined(VISP_HAVE_DISPLAY) && defined(VISP_HAVE_MODULE_GUI)
    if ((!m_display_initialized) && display_frame) {
      m_display = vpDisplayFactory::createDisplay(m_I);
      m_display_initialized = true;
    }
    if (m_display_initialized) {
      if (display_frame) {
        const unsigned int v_offset = 30;
        unsigned int r = 1;
        std::string left_click_text = std::string("Click left to turn ") + (m_has_to_track ? std::string("OFF") : std::string("ON")) + std::string(" the tracking.");
        std::string right_click_text("Click right to kill the node.");
        vpImagePoint ip_left_click(m_I.getHeight() - v_offset * r, s_hor_offset_from_left_border);
        ++r;
        vpImagePoint ip_right_click(m_I.getHeight() - v_offset * r, s_hor_offset_from_left_border);
        vpFont font(20, vpFont::GENERIC_MONOSPACE);
        font.drawText(m_I, left_click_text, ip_left_click, 0, 255);
        font.drawText(m_I, right_click_text, ip_right_click, 0, 255);
        vpDisplay::display(m_I);
      }
    }
#endif

    if (has_to_track) {
      double t_start = vpTime::measureTimeMs();
      bool found = m_tag_detector.detect(m_I);
      double t_end_tracking = vpTime::measureTimeMs();
      std::vector<std::string> vec_info; // Vector that contains info to display on screen
      static const unsigned int nb_digits = 1; // Number of digits to display doubles on screen
      std::string t_string = std::to_string(t_end_tracking - t_start);
      std::string tracking_time = "Tracking time: " + t_string.substr(0, t_string.find(".") + nb_digits + 1) + "ms";
      vec_info.push_back(tracking_time);

      if (found) {
        size_t nb_objects = m_tag_detector.getNbObjects();
        RCLCPP_DEBUG(this->get_logger(), "Detected %ld AprilTag(s)", nb_objects);
        auto decision_margins = m_tag_detector.getTagsDecisionMargin();
        auto tags_IDs = m_tag_detector.getTagsId();
        auto tags_corners = m_tag_detector.getTagsCorners();
        visp_tracker_common::msg::AprilTagDetectionArray detectionArray;
        detectionArray.header = msg->header;

        for (size_t i = 0; i < nb_objects; ++i) {
          {
            std::stringstream ss;
            std::string margin_as_str = std::to_string(decision_margins[i]);
            ss << "Tag " << i << ": margin = " << margin_as_str.substr(0, margin_as_str.find(".") + nb_digits + 1);
            vec_info.push_back(ss.str());
          }

          int tag_ID = tags_IDs[i];
          float tag_size;
          auto idx_tag_size = m_tag_size_map.find(tag_ID);
          if (idx_tag_size != m_tag_size_map.end()) {
            tag_size = idx_tag_size->second;
          }
          else {
            tag_size = m_default_tag_size;
          }

          vpHomogeneousMatrix c_M_o;
          m_tag_detector.getPose(i, tag_size, m_rgb_cam, c_M_o);

          RCLCPP_DEBUG_STREAM(this->get_logger(), "Tag " << i << " with size " << tag_size << " with margin " << decision_margins[i] << " and pose:\n" << c_M_o);

          auto tag_corners = tags_corners[i];

#if defined(VISP_HAVE_DISPLAY) && defined(VISP_HAVE_MODULE_GUI)
          if (m_display_initialized && display_frame) {
            unsigned int thickness = 2;
            vpDisplay::displayFrame(m_I, c_M_o, m_rgb_cam, 0.1, vpColor::none, thickness);
          }
#endif

          geometry_msgs::msg::Pose pose_c_M_o = visp_common::pose::toGeometryMsgsPose(c_M_o);

          visp_tracker_common::msg::AprilTagDetection detectionMsg;
          detectionMsg.header = msg->header;
          detectionMsg.family = m_family_name;
          detectionMsg.id = tags_IDs[i];
          detectionMsg.size = tag_size;
          detectionMsg.pose = pose_c_M_o;
          vpImagePoint cog = m_tag_detector.getCog(i);
          fromImagePoint(cog, detectionMsg.center);
          fromImagePoint(tag_corners[0], detectionMsg.corners[0]);
          fromImagePoint(tag_corners[1], detectionMsg.corners[1]);
          fromImagePoint(tag_corners[2], detectionMsg.corners[2]);
          fromImagePoint(tag_corners[3], detectionMsg.corners[3]);
          // TODO find a way to get the alignment parameter with vpDetectorAprilTag::isZAlignedWithCameraAxis()
          detectionMsg.z_axis_aligned_with_camera_axis = m_tag_detector.isZAlignedWithCameraAxis();
          static const unsigned int nb_tag_corners = 4;
          std::vector<double> tag_corners_X(nb_tag_corners);
          std::vector<double> tag_corners_Y(nb_tag_corners);
          std::vector<double> tag_corners_Z(nb_tag_corners, 0.);
          if (detectionMsg.z_axis_aligned_with_camera_axis) {
            tag_corners_X = { -detectionMsg.size / 2., +detectionMsg.size / 2., +detectionMsg.size / 2., -detectionMsg.size / 2. };
            tag_corners_Y = { +detectionMsg.size / 2., +detectionMsg.size / 2., -detectionMsg.size / 2., -detectionMsg.size / 2. };
          }
          else {
            // Case where tag z-axis is NOT aligned with camera z-axis
            tag_corners_X = { -detectionMsg.size / 2., +detectionMsg.size / 2., +detectionMsg.size / 2., -detectionMsg.size / 2. };
            tag_corners_Y = { -detectionMsg.size / 2., -detectionMsg.size / 2., +detectionMsg.size / 2., +detectionMsg.size / 2. };
          }
          for (unsigned int i = 0; i < nb_tag_corners; ++i) {
            detectionMsg.corners_3d.at(i) = geometry_msgs::msg::Point().set__x(tag_corners_X[i]).set__y(tag_corners_Y[i]).set__z(tag_corners_Z[i]);
          }
          detectionArray.detections.push_back(detectionMsg);

          if (m_opt_id) {
            if (tags_IDs[i] == *m_opt_id) {
              geometry_msgs::msg::PoseStamped stamped_pose;
              stamped_pose.header = msg->header;
              stamped_pose.pose = pose_c_M_o;
              m_poses_pub->publish(stamped_pose);
            }
          }

          if (m_is_headless_mode && m_visualization_debug) {
            // Publish the tag corners
            visp_tracker_common::msg::NamedFeature feature2D;
            feature2D.name = "Tag_" + std::to_string(tags_IDs[i]);
            for (unsigned int i = 0; i < 3; ++i) {
              vision_msgs::msg::Point2D start, end;
              fromImagePoint(tag_corners[i], start);
              fromImagePoint(tag_corners[i+1], end);
              feature2D.lines.push_back(visp_tracker_common::msg::Point2DTuple().set__start(start).set__end(end));
            }
            vision_msgs::msg::Point2D start, end;
            fromImagePoint(tag_corners[3], start);
            fromImagePoint(tag_corners[0], end);
            feature2D.lines.push_back(visp_tracker_common::msg::Point2DTuple().set__start(start).set__end(end));
            visp_tracker_common::msg::NamedFeatureArray namedFeaturesMsg;
            namedFeaturesMsg.features.push_back(feature2D);
            m_features_pub->publish(namedFeaturesMsg);
          }
        }
        // m_poses_pub->publish(stamped_pose);
        m_tags_info_pub->publish(detectionArray);
      }

      // Manage info strings publication
      if (m_info_strings.info_strings.size() == m_info_nb_static) {
        m_info_strings.info_strings.insert(m_info_strings.info_strings.end(), vec_info.begin(), vec_info.end());
      }
      else {
        unsigned int nb_infos = vec_info.size();
        m_info_strings.info_strings.resize(m_info_nb_static + nb_infos);
        for (unsigned int i = 0; i < nb_infos; ++i) {
          m_info_strings.info_strings[m_info_nb_static + i] = vec_info[i];
        }
      }
      m_info_strings.hor_offset_right_border.resize(m_info_strings.info_strings.size(), BaseTracker::s_hor_offset_from_left_border);
      m_info_strings_pub->publish(m_info_strings);
    }

#if defined(VISP_HAVE_DISPLAY) && defined(VISP_HAVE_MODULE_GUI)
    if (m_display_initialized && display_frame) {
      unsigned int nb_infos = m_info_strings.info_strings.size();
      const unsigned int v_offset = 20;
      for (unsigned int r = 0; r < nb_infos; ++r) {
        vpDisplay::displayText(m_I, v_offset * (r + 1), m_info_strings.hor_offset_right_border[r], m_info_strings.info_strings[r], vpColor::red);
      }
      vpDisplay::flush(m_I);
    }

    // Getting user interaction feedback
    vpMouseButton::vpMouseButtonType button;
    if (vpDisplay::getClick(m_I, button, false)) {
      switch (button) {
      case vpMouseButton::button1:
      {
        std::scoped_lock lock(m_mutex_tracking);
        m_has_to_track = (!m_has_to_track);
        break;
      }
      case vpMouseButton::button3:
      {
        {
          std::scoped_lock lock(m_mutex_tracking);
          m_has_to_track = false;
        }
        {
          std::scoped_lock lock(m_mutex_quit);
          m_quit = true;
        }
        break;
      }
      default:
        break;
      }
    }
#endif

    ++m_frame_cnt;
  }
  catch (const std::exception &e) {
    RCLCPP_ERROR(this->get_logger(), "Image callback exception: %s", e.what());
  }
}

//////////////////////////////////////////////////////////////////////
//                        OTHERS                                    //
//////////////////////////////////////////////////////////////////////

}
