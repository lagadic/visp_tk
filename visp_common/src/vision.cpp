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
  \file vision.cpp
  \brief Conversions between ROS and ViSP structures representing vision parameters
*/

#include <visp_common/vision.hpp>

namespace visp_common
{
namespace vision
{
vision_msgs::msg::Point2D toVisionMsgsPoint2D(const vpImagePoint &ip)
{
  return (vision_msgs::msg::Point2D().set__x(ip.get_u()).set__y(ip.get_v()));
}

vpImagePoint toVispImagePoint(const vision_msgs::msg::Point2D &point_2d)
{
  return vpImagePoint(point_2d.y, point_2d.x);
}
}
}
