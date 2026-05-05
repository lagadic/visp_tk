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
  \file vision.hpp
  \brief Conversions between ROS and ViSP structures representing vision messages.
*/

#ifndef VISP_COMMON__VISION_H_
#define VISP_COMMON__VISION_H_

#include <vision_msgs/msg/point2_d.hpp>

#include <visp3/core/vpImagePoint.h>

#ifdef ENABLE_VISP_NAMESPACE
using namespace VISP_NAMESPACE_NAME;
#endif

namespace visp_common
{
namespace vision
{
/**
 * \brief Converts a ViSP image point to a vision_msgs::Point2D.
 * \param[in] ip Image point in ViSP format.
 * \return Image point in ROS/vision_msgs format.
*/
vision_msgs::msg::Point2D toVisionMsgsPoint2D(const vpImagePoint &ip);


/**
 * \brief Converts a vision_msgs::Point2D to a ViSP image point.
 * \param[in] point_2d Image point in vision_msgs::Point2D format
 * \return Image point in ViSP format.
*/
vpImagePoint toVispImagePoint(const vision_msgs::msg::Point2D &point_2d);
}
}

#endif
