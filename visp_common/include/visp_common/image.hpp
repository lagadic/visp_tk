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
  \file image.hpp
  \brief Defines conversions between ViSP and ROS image types
 */

#ifndef VISP_COMMON__IMAGE_H__
#define VISP_COMMON__IMAGE_H__

#include <sensor_msgs/msg/image.hpp>

#include <visp3/core/vpImage.h>
#include <visp3/core/vpRGBa.h>

namespace visp_common
{
namespace image
{
/**
 * \brief Converts a ViSP image (vpImage) to a sensor_msgs::Image.
 * \param[in] src Image in ViSP format.
 * \return Image in ROS/sensor_msgs format.
*/
sensor_msgs::msg::Image toSensorMsgsImage(const VISP_NAMESPACE_ADDRESSING vpImage<unsigned char> &src);
/**
 * \brief Converts a ViSP image (vpImage) to a sensor_msgs::Image.
 * \param[in] src Image in ViSP format.
 * \return Image in ROS/sensor_msgs format.
*/
sensor_msgs::msg::Image toSensorMsgsImage(const VISP_NAMESPACE_ADDRESSING vpImage<uint16_t> &src);
/**
 * \brief Converts a ViSP image (vpImage) to a sensor_msgs::Image.
 * \param[in] src Image in ViSP format.
 * \return Image in ROS/sensor_msgs format.
*/
sensor_msgs::msg::Image toSensorMsgsImage(const VISP_NAMESPACE_ADDRESSING vpImage<VISP_NAMESPACE_ADDRESSING vpRGBa> &src);

/**
 * \brief Converts a sensor_msgs::Image to a ViSP image (vpImage).
 * \param[in] src Image in ROS/sensor_msgs format.
 * \return Image in ViSP format.
*/
VISP_NAMESPACE_ADDRESSING vpImage<unsigned char> toVispImageChar(const sensor_msgs::msg::Image &src);

/**
 * \brief Converts a sensor_msgs::Image to a ViSP image (vpImage).
 * \param[in] src Image in ROS/sensor_msgs format.
 * \return vpImage<uint16_t> The ViSP image, encoded using uint16_t
 */
VISP_NAMESPACE_ADDRESSING vpImage<uint16_t> toVispImageUint16(const sensor_msgs::msg::Image &src);

/**
 * \brief Converts a sensor_msgs::Image to a ViSP image (vpImage).
 * \param[in] src Image in ROS/sensor_msgs format.
 * \return vpImage<vpRGBa> The ViSP color image.
 */
VISP_NAMESPACE_ADDRESSING vpImage<VISP_NAMESPACE_ADDRESSING vpRGBa> toVispImageRGBa(const sensor_msgs::msg::Image &src);
} // namespace image
} // namespace visp_common

#endif // VISP_COMMON__IMAGE_H__
