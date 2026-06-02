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

#ifndef APRILTAG_TRACKER_HPP
#define APRILTAG_TRACKER_HPP

#include <sensor_msgs/msg/image.hpp>
#include <visp_common/image.hpp>
#include <visp_common/pose.hpp>
#include <visp_tracker_common/BaseTracker.hpp>
#include <visp_tracker_common/msg/april_tag_detection_array.hpp>

#include <visp3/core/vpIoTools.h>
#if defined(VISP_HAVE_DISPLAY) && defined(VISP_HAVE_MODULE_GUI)
#include <visp3/core/vpFont.h>
#include <visp3/gui/vpDisplayFactory.h>
#endif
#include <visp3/detection/vpDetectorAprilTag.h>

#include <map>

#ifdef ENABLE_VISP_NAMESPACE
using namespace VISP_NAMESPACE_NAME;
#endif

namespace visp_apriltag
{
class AprilTagTracker : public visp_tracker_common::BaseTracker
{
public:
  AprilTagTracker(const std::string &node_name);
  virtual ~AprilTagTracker() = default;

protected:
  /** @name  Initialization */
  ///@{

  /**
   * @copydoc visp_tracker_common::BaseTracker::init_tracker()
   */
  virtual bool init_tracker() override;

  /**
   * @copydoc visp_tracker_common::BaseTracker::init_info_strings()
   */
  virtual void init_info_strings() override;

  ///@}

  /** @name  Callbacks */
  ///@{

  // ----- Subscriptions -----

  /**
   * @brief Color image callback.
   *
   * @param msg The color image.
   */
  void image_callback(const sensor_msgs::msg::Image::ConstSharedPtr &msg);

  // ----- Services -----

  ///@}

  // ----- Services -----

  // ----- Subscribers -----
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr m_rgb_stream_sub; //!< RGB stream subscriber

  // ----- Publisher -----
  rclcpp::Publisher<visp_tracker_common::msg::AprilTagDetectionArray>::SharedPtr m_tags_info_pub; //!< Publisher of the tag IDs, centers and so on.

  // ----- Display-related attributes -----
#if defined(VISP_HAVE_DISPLAY) && defined(VISP_HAVE_MODULE_GUI)
  bool m_display_initialized = false;
  std::shared_ptr<vpDisplay> m_display;
#endif

  // ----- Tracking-related attributes -----
  vpDetectorAprilTag m_tag_detector; //!< The detector.
  std::string m_family_name; //!< Name of the tag family.
  std::map<int64_t, float> m_tag_size_map; //!< ID and associated size of the tag, in meters. ID -1 is a special case that means "any ID that is not explicitly listed in the map".
  float m_default_tag_size; //!< Tag size for any tag ID that is not list in m_tag_size_map.
  vpImage<unsigned char> m_I; //!< Color image on which the detection is performed.
  std::optional<int> m_opt_id = std::nullopt; //!< If set, the ID of the tag whose pose must be published
};
}

#endif
