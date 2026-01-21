/*
 * This file is part of the ViSP software.
 * Copyright (C) 2005 - 2026 by INRIA. All rights reserved.
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * ("GPL") version 2 as published by the Free Software Foundation.
 * See the file LICENSE.txt at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * For using ViSP with software that can not be combined with the GNU
 * GPL, please contact INRIA about acquiring a ViSP Professional
 * Edition License.
 *
 * See https://visp.inria.fr for more information.
 *
 * This software was developed at:
 * INRIA Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 * https://team.inria.fr/rainbow/
 *
 * If you have questions regarding the use of this file, please contact
 * INRIA at visp@inria.fr
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Contact visp@irisa.fr if any conditions of this licensing are
 * not clear to you.
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
sensor_msgs::msg::Image toSensorMsgsImage(const vpImage<unsigned char> &src);
/**
 * \brief Converts a ViSP image (vpImage) to a sensor_msgs::Image.
 * \param[in] src Image in ViSP format.
 * \return Image in ROS/sensor_msgs format.
*/
sensor_msgs::msg::Image toSensorMsgsImage(const vpImage<uint16_t> &src);
/**
 * \brief Converts a ViSP image (vpImage) to a sensor_msgs::Image.
 * \param[in] src Image in ViSP format.
 * \return Image in ROS/sensor_msgs format.
*/
sensor_msgs::msg::Image toSensorMsgsImage(const vpImage<vpRGBa> &src);

/**
 * \brief Converts a sensor_msgs::Image to a ViSP image (vpImage).
 * \param[in] src Image in ROS/sensor_msgs format.
 * \return Image in ViSP format.
*/
vpImage<unsigned char> toVispImageChar(const sensor_msgs::msg::Image &src);

/**
 * \brief Converts a sensor_msgs::Image to a ViSP image (vpImage).
 * \param[in] src Image in ROS/sensor_msgs format.
 * \return vpImage<uint16_t> The ViSP image, encoded using uint16_t
 */
vpImage<uint16_t> toVispImageUint16(const sensor_msgs::msg::Image &src);

/**
 * \brief Converts a sensor_msgs::Image to a ViSP image (vpImage).
 * \param[in] src Image in ROS/sensor_msgs format.
 * \return vpImage<vpRGBa> The ViSP color image.
 */
vpImage<vpRGBa> toVispImageRGBa(const sensor_msgs::msg::Image &src);
} // namespace image
} // namespace visp_common

#endif // VISP_COMMON__IMAGE_H__
