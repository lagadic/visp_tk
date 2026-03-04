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
  auto tag_size_param_desc = rcl_interfaces::msg::ParameterDescriptor {};
  tag_size_param_desc.description = "This parameter indicates the size of the tag, expressed in meters. See https://visp-doc.inria.fr/doxygen/visp-daily/classvpDetectorAprilTag.html for more information";
  this->declare_parameter<double>("tag_size", tag_size_param_desc);

  auto id_pub_param_desc = rcl_interfaces::msg::ParameterDescriptor {};
  id_pub_param_desc.description = "If set, the ID of the tag whose pose must be published on the pose topic.";
  this->declare_parameter<int>("id_published", -1, id_pub_param_desc);

  auto tag_family_param_desc = rcl_interfaces::msg::ParameterDescriptor {};
  tag_family_param_desc.description = "This parameter indicates the family of the tag. Available families are " + vpDetectorAprilTag::getAvailableTagFamily();
  this->declare_parameter<std::string>("tag_family", tag_family_param_desc);

  auto pose_method_param_desc = rcl_interfaces::msg::ParameterDescriptor {};
  pose_method_param_desc.description = "This parameter indicates the pose estimation to use when a tag is detected. Available methods are " + vpDetectorAprilTag::getAvailablePoseMethod();
  this->declare_parameter<std::string>("pose_method", pose_method_param_desc);

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

  if (m_config_file.empty()) {
    auto tag_size_param = rclcpp::Parameter();
    bool isSet = this->get_parameter("tag_size", tag_size_param);
    if (isSet) {
      m_tag_size = static_cast<float>(tag_size_param.as_double());
      RCLCPP_INFO(this->get_logger(), "Tag size is set to %f\n", m_tag_size);
    }
    else {
      RCLCPP_ERROR(this->get_logger(), "%s parameter is not set", tag_size_param.get_name().c_str());
      return false;
    }

    auto tag_family_param = rclcpp::Parameter();
    isSet = this->get_parameter("tag_family", tag_family_param);
    if (isSet) {
      m_family_name = tag_family_param.as_string();
      RCLCPP_INFO(this->get_logger(), "Tag family is set to %s\n", m_family_name.c_str());
    }
    else {
      RCLCPP_ERROR(this->get_logger(), "%s parameter is not set", tag_family_param.get_name().c_str());
      return false;
    }

    try {
      vpDetectorAprilTag::vpAprilTagFamily tag_family = vpDetectorAprilTag::tagFamilyFromString(m_family_name);
      m_tag_detector.setAprilTagFamily(tag_family);
    }
    catch (const vpException &e) {
      RCLCPP_ERROR(this->get_logger(), "%s parameter value '%s' cannot be converted to a known family. Allowed values are: %s", tag_family_param.get_name().c_str(), m_family_name.c_str(), vpDetectorAprilTag::getAvailableTagFamily().c_str());
      return false;
    }

    auto pose_method_param = rclcpp::Parameter();
    this->get_parameter("pose_method", pose_method_param);
    auto pose_method_name = pose_method_param.as_string();
    try {
      vpDetectorAprilTag::vpPoseEstimationMethod pose_method = vpDetectorAprilTag::poseMethodFromString(pose_method_name);
      m_tag_detector.setAprilTagPoseEstimationMethod(pose_method);
      RCLCPP_INFO(this->get_logger(), "Pose estimation method is set to %s\n", pose_method_name.c_str());
    }
    catch (const vpException &e) {
      RCLCPP_ERROR(this->get_logger(), "%s parameter value '%s' cannot be converted to a known family. Allowed values are: %s", pose_method_param.get_name().c_str(), pose_method_name.c_str(), vpDetectorAprilTag::getAvailablePoseMethod().c_str());
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

    RCLCPP_INFO(this->get_logger(), "Done !");
    return true;
  }
  else {
    try {
      m_tag_detector.loadConfigFile(m_config_file);
    }
    catch (const vpException &e) {
      RCLCPP_ERROR(this->get_logger(), e.what());
      return false;
    }
    return true;
  }
}

void AprilTagTracker::init_info_strings()
{
  const unsigned int nb_digits = 3;
  m_info_strings.info_strings.push_back(std::string("Tag Size.........: ") + std::to_string(m_tag_size).substr(0, std::to_string(m_tag_size).find(".") + nb_digits + 1));
  m_info_strings.hor_offset_right_border.push_back(1.5 * s_default_hor_offset);
  m_info_strings.info_strings.push_back(std::string("Tag family.......: ") + m_family_name);
  m_info_strings.hor_offset_right_border.push_back(1.5 * s_default_hor_offset);
  m_info_strings.info_strings.push_back(std::string("Margin threshold.: ") + (m_tag_detector.getAprilTagDecisionMarginThreshold() < 0 ? std::string("deactivated") : std::to_string(m_tag_detector.getAprilTagDecisionMarginThreshold())));
  m_info_strings.hor_offset_right_border.push_back(1.5 * s_default_hor_offset);
  m_info_nb_static = m_info_strings.info_strings.size();
}

//////////////////////////////////////////////////////////////////////
//                        ROS2 SUBCRIPTIONS                         //
//////////////////////////////////////////////////////////////////////

void AprilTagTracker::image_callback(const sensor_msgs::msg::Image::ConstSharedPtr &msg)
{
  RCLCPP_DEBUG(this->get_logger(), "Receive image");
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
    if (quit || (!m_rgb_cam_info_received) || (!has_to_track)) {
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
        vpDisplay::display(m_I);
        static const int vert_offset = 20;
        const int nb_info = m_info_strings.info_strings.size();
        for (int idx = 0; idx < nb_info; ++idx) {
          vpDisplay::displayText(m_I, vert_offset * (idx + 1), m_I.getWidth() - m_info_strings.hor_offset_right_border[idx], m_info_strings.info_strings[idx], vpColor::red);
        }
      }
    }
#endif

    std::vector<vpHomogeneousMatrix> c_M_o_vec;
    double t_start = vpTime::measureTimeMs();
    bool found = m_tag_detector.detect(m_I, m_tag_size, m_rgb_cam, c_M_o_vec);
    double t_end_tracking = vpTime::measureTimeMs();
    std::vector<std::string> vec_info; // Vector that contains info to display on screen
    static const unsigned int nb_digits = 2; // Number of digits to display doubles on screen
    std::string t_string = std::to_string(t_end_tracking - t_start);
    std::string tracking_time = "Tracking time: " + t_string.substr(0, t_string.find(".") + nb_digits + 1) + "ms";
    vec_info.push_back(tracking_time);

    vpColVector v_ee(6, 0);
    if (found) {
      RCLCPP_DEBUG(this->get_logger(), "Detected %ld AprilTag(s)", c_M_o_vec.size());
      auto decision_margins = m_tag_detector.getTagsDecisionMargin();
      auto tags_IDs = m_tag_detector.getTagsId();
      auto tags_corners = m_tag_detector.getTagsCorners();
      visp_tracker_common::msg::AprilTagDetectionArray detectionArray;
      detectionArray.header = msg->header;

      for (size_t i = 0; i < c_M_o_vec.size(); ++i) {
        RCLCPP_DEBUG_STREAM(this->get_logger(), "Tag " << i << " with size " << m_tag_size << " with margin " << decision_margins[i] << " and pose:\n" << c_M_o_vec[i]);
        {
          std::stringstream ss;
          ss << "Tag " << i << ": margin = " << decision_margins[i];
          vec_info.push_back(ss.str());
        }

        vpHomogeneousMatrix c_M_o = c_M_o_vec[i];
        auto tag_corners = tags_corners[i];

#if defined(VISP_HAVE_DISPLAY) && defined(VISP_HAVE_MODULE_GUI)
        if (m_display_initialized && display_frame) {
          vpDisplay::displayFrame(m_I, c_M_o, m_rgb_cam, 0.1);
        }
#endif

        geometry_msgs::msg::Pose pose_c_M_o = visp_common::pose::toGeometryMsgsPose(c_M_o);
        geometry_msgs::msg::PoseStamped stamped_pose;
        stamped_pose.pose = pose_c_M_o;
        stamped_pose.header.frame_id = msg->header.frame_id;
        stamped_pose.header.stamp = this->get_clock()->now();

        visp_tracker_common::msg::AprilTagDetection detectionMsg;
        detectionMsg.family = m_family_name;
        detectionMsg.id = tags_IDs[i];
        detectionMsg.size = m_tag_size;
        detectionMsg.pose = stamped_pose;
        vpImagePoint cog = m_tag_detector.getCog(i);
        fromImagePoint(cog, detectionMsg.center);
        fromImagePoint(tag_corners[0], detectionMsg.corners[0]);
        fromImagePoint(tag_corners[1], detectionMsg.corners[1]);
        fromImagePoint(tag_corners[2], detectionMsg.corners[2]);
        fromImagePoint(tag_corners[3], detectionMsg.corners[3]);
        detectionArray.detections.push_back(detectionMsg);

        if (m_opt_id) {
          if (tags_IDs[i] == *m_opt_id) {
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
    m_info_strings.hor_offset_right_border.resize(m_info_strings.info_strings.size(), 1.5 * BaseTracker::s_default_hor_offset);
    m_info_strings_pub->publish(m_info_strings);

#if defined(VISP_HAVE_DISPLAY) && defined(VISP_HAVE_MODULE_GUI)
    if (m_display_initialized && display_frame) {
      unsigned int nb_infos = m_info_strings.info_strings.size();
      const unsigned int v_offset = 20;
      for (unsigned int r = 0; r < nb_infos; ++r) {
        vpDisplay::displayText(m_I, v_offset * (r + 1), m_I.getWidth() - m_info_strings.hor_offset_right_border[r], m_info_strings.info_strings[r], vpColor::red);
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
