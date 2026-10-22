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

#ifndef SEQUENCE_PLAYER_HPP
#define SEQUENCE_PLAYER_HPP

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include <sensor_msgs/msg/image.hpp>

#include <visp_common/camera.hpp>
#include <visp_common/image.hpp>
#include <visp_common/path_retriever.hpp>
#include <visp_common/pose.hpp>

#include <visp3/core/vpConfig.h>
#include <visp3/core/vpCameraParameters.h>
#include <visp3/core/vpException.h>
#include <visp3/core/vpHomogeneousMatrix.h>
#include <visp3/core/vpImage.h>
#include <visp3/core/vpImageConvert.h>
#include <visp3/core/vpIoTools.h>
#include <visp3/core/vpRGBa.h>

#include <visp3/io/vpImageIo.h>

namespace visp_tk_tutorials
{

using VISP_NAMESPACE_NAME::vpCameraParameters;
using VISP_NAMESPACE_NAME::vpHomogeneousMatrix;

class SequencePlayer : public rclcpp::Node
{
public:

  SequencePlayer(const std::string &name);

  bool init();

  bool has_to_quit()
  {
    return m_has_to_quit;
  }

  void stop_and_quit()
  {
    m_has_to_quit = true;
  }

private:
  struct SequenceFrame
  {
    sensor_msgs::msg::Image IRGB;
    sensor_msgs::msg::Image depth;
  };

  inline unsigned int getImageHeight() const { return m_h; }
  inline unsigned int getImageWidth() const { return m_w; }
  inline vpCameraParameters cam() const { return m_cam; }

  inline unsigned int numFrames() const
  {
    return m_frames.size();
  }

  inline SequenceFrame &getFrame(unsigned int index)
  {
    return m_frames[index];
  }

  bool loadCameraSettings();

  const std::string getColorFrame(unsigned int index);
  const std::string getDepthFrame(unsigned int index);
  bool loadFrames();
  bool getInitialPoses();

  void periodic_publication();

  bool m_has_to_quit = false;
  std::string m_path;
  std::string m_object_name;
  rclcpp::Parameter m_depth_camera_info_topic_name;
  rclcpp::Parameter m_depth_stream_name;
  rclcpp::Parameter m_rgb_camera_info_topic_name;
  rclcpp::Parameter m_rgb_stream_name;
  rclcpp::Parameter m_init_pose_topic_name;

  vpCameraParameters m_cam;
  sensor_msgs::msg::CameraInfo m_cam_info;
  float m_depthScale;
  unsigned int m_h, m_w;
  std::vector<SequenceFrame> m_frames;

  vpHomogeneousMatrix m_initial_cMo;
  geometry_msgs::msg::PoseStamped m_initial_pose;

  rclcpp::Publisher<sensor_msgs::msg::CameraInfo>::SharedPtr m_depth_cam_info_pub; //!< Depth camera parameters publisher
  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr m_depth_img_pub; //!< Depth camera parameters publisher
  rclcpp::Publisher<sensor_msgs::msg::CameraInfo>::SharedPtr m_rgb_cam_info_pub; //!< RGB camera parameters publisher
  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr m_rgb_img_pub; //!< RGB camera parameters publisher
  rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr m_init_pose_pub; //!< Publisher of the init topic

  rclcpp::TimerBase::SharedPtr m_timer;
};
}

#endif
