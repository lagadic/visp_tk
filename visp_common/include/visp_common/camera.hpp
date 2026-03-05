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

/*!
  \file camera.hpp
  \brief Conversions between ROS and ViSP structures representing camera parameters
*/

#ifndef VISP_COMMON__CAMERA_H_
#define VISP_COMMON__CAMERA_H_

#include <sensor_msgs/msg/camera_info.hpp>

#include <visp3/core/vpCameraParameters.h>

#ifdef ENABLE_VISP_NAMESPACE
using namespace VISP_NAMESPACE_NAME;
#endif

namespace visp_common
{
namespace camera
{
/*!
  \brief Converts a sensor_msgs::CameraInfo to ViSP camera parameters (vpCameraParameters).
  \param[in] cam_info Camera parameters in ROS/sensor_msgs format.
  \return camera parameters in ViSP format.
*/
vpCameraParameters toVispCameraParameters(const sensor_msgs::msg::CameraInfo &cam_info);
/*!
  \brief Converts a sensor_msgs::CameraInfo::ConstSharedPtr to ViSP camera parameters (vpCameraParameters).
  \param[in] cam_info Camera parameters in ROS/sensor_msgs format.
  \return camera parameters in ViSP format.
*/
vpCameraParameters toVispCameraParameters(const sensor_msgs::msg::CameraInfo::ConstSharedPtr &cam_info);
/*!
  \brief Converts ViSP camera parameters (vpCameraParameters) to sensor_msgs::CameraInfo.
  \param[in] cam_info Camera parameters in ViSP format.
  \param[in] cam_image_width x-resolution of the camera image
  \param[in] cam_image_height y-resolution of the camera image
  \return Camera parameters in ROS/sensor_msgs format.
*/
sensor_msgs::msg::CameraInfo toSensorMsgsCameraInfo(vpCameraParameters &cam_info, unsigned int cam_image_width,
                                                    unsigned int cam_image_height);
} // namespace camera
} // namespace visp_common

#endif // VISP_COMMON__CAMERA_H_
