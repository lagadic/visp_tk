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
  \file pose.hpp
  \brief Conversions between ROS and ViSP structures representing a 3D pose
*/

#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/transform.hpp>

#include <visp3/core/vpHomogeneousMatrix.h>

#ifndef VISP_COMMON__3DPOSE_H_
#define VISP_COMMON__3DPOSE_H_

#ifdef ENABLE_VISP_NAMESPACE
using namespace VISP_NAMESPACE_NAME;
#endif

namespace visp_common
{
namespace pose
{
/*!
  \brief Converts a geometry_msgs::msg::Transform to a ViSP homogeneous matrix (vpHomogeneousMatrix).
  \param[in] trans Homogeneous transformation in ROS/geometry_msgs format.
  \return Corresponding transformation in ViSP format.
*/
vpHomogeneousMatrix toVispHomogeneousMatrix(const geometry_msgs::msg::Transform &trans);
/*!
  \brief Converts a geometry_msgs::msg::Transform to a ViSP homogeneous matrix (vpHomogeneousMatrix).
  \param[in] trans Homogeneous transformation in ROS/geometry_msgs format.
  \return Corresponding transformation in ViSP format.
*/
vpHomogeneousMatrix toVispHomogeneousMatrix(const geometry_msgs::msg::Transform::ConstSharedPtr &trans);

/*!
  \brief Converts a geometry_msgs::msg::Pose to a ViSP homogeneous matrix (vpHomogeneousMatrix).
  \param[in] pose Homogeneous transformation in ROS/geometry_msgs format.
  \return Corresponding transformation in ViSP format.
*/
vpHomogeneousMatrix toVispHomogeneousMatrix(const geometry_msgs::msg::Pose &pose);

/*!
  \brief Converts a geometry_msgs::msg::Pose to a ViSP homogeneous matrix (vpHomogeneousMatrix).
  \param[in] pose Homogeneous transformation in ROS/geometry_msgs format.
  \return Corresponding transformation in ViSP format.
*/
vpHomogeneousMatrix toVispHomogeneousMatrix(const geometry_msgs::msg::Pose::ConstSharedPtr &pose);

/*!
  \brief Converts a ViSP homogeneous matrix (vpHomogeneousMatrix) to a geometry_msgs::msg::Transform.
  \param[in] mat Homogeneous transformation in ViSP format.
  \return: transformation in ROS/geometry_msgs format.
*/
geometry_msgs::msg::Transform toGeometryMsgsTransform(const vpHomogeneousMatrix &mat);

/*!
        \brief Converts a ViSP homogeneous matrix (vpHomogeneousMatrix) to a geometry_msgs::msg::Pose.
        \param[in] mat Homogeneous transformation in ViSP format.
        \return: transformation in ROS/geometry_msgs format.
      */
geometry_msgs::msg::Pose toGeometryMsgsPose(const vpHomogeneousMatrix &mat);
} // namespace pose
} // namespace visp_common

#endif // VISP_COMMON__3DPOSE_H_
