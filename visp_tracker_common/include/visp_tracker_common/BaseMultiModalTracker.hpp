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

#ifndef BASE_MM_TRACKER_HPP
#define BASE_MM_TRACKER_HPP

#include <rclcpp/rclcpp.hpp>

#if __has_include(<message_filters/subscriber.hpp>)
#include <message_filters/subscriber.hpp>
#include <message_filters/synchronizer.hpp>
#include <message_filters/sync_policies/approximate_time.hpp>
#else
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#endif

#include <visp_tracker_common/BaseTracker.hpp>
#include <visp_tracker_common/names.hpp>
#include <visp_tracker_common/msg/named_feature.hpp>

namespace visp_tracker_common
{
class BaseMultiModalTracker : public BaseTracker
{
public:
  BaseMultiModalTracker(const std::string &name, const bool &does_publish_features);
  virtual ~BaseMultiModalTracker() = default;

  /**
   * @copydoc visp_tracker_common::BaseTracker::init()
   */
  virtual bool init() override;

protected:
  /** @name  Initialization */
  ///@{

  /**
   * @brief Check the tracker settings in order to know if the depth
   * is actually required and set m_depth_is_required accordingly.
   */
  virtual void check_requires_depth() = 0;

  ///@}


  /** @name  Callbacks */
  ///@{

  // ----- Subscriptions -----

  /**
   * @brief Color camera parameters callback that is called only once to
   * initialize the node internal parameters
   *
   * @param msg Color camera parameters message
   */
  virtual void depth_camera_info_callback(const sensor_msgs::msg::CameraInfo::SharedPtr msg);

  /**
   * @brief Callback called when the tracker only requires a RGB stream.
   *
   * @param rgb Message from the RGB stream.
   */
  virtual void rgb_callback(const sensor_msgs::msg::Image::ConstSharedPtr &rgb);

  /**
   * @brief Callback called when the tracker requires both a RGB stream and a depth stream.
   *
   * @param rgb Message from the RGB stream.
   * @param depth Message from the depth stream.
   */
  virtual void sync_callback(const sensor_msgs::msg::Image::ConstSharedPtr &rgb, const sensor_msgs::msg::Image::ConstSharedPtr &depth);

  // ----- Services -----

  ///@}

  /**
   * @brief Convert the RGB stream into usable data.
   *
   * @param rgb The RGB stream message.
   */
  virtual void treat_rgb(const sensor_msgs::msg::Image::ConstSharedPtr &rgb) = 0;

  /**
   * @brief Treat the depth stream into usable data.
   *
   * @param depth The depth stream message.
   */
  virtual void treat_depth(const sensor_msgs::msg::Image::ConstSharedPtr &depth) = 0;

  /**
   * @brief Perform the tracking.
   *
   */
  virtual void track() = 0;

  // ----- Parameters changes handling -----

  // ----- Services -----

  // ----- Subscribers -----
  rclcpp::Subscription<sensor_msgs::msg::CameraInfo>::SharedPtr m_depth_cam_info_sub; //!< Depth camera parameters subscriber
  message_filters::Subscriber<sensor_msgs::msg::Image> m_rgb_stream_sub; //!< RGB stream subscriber when depth is used
  message_filters::Subscriber<sensor_msgs::msg::Image> m_depth_stream_sub; //!< Depth stream subscriber when depth is used
  std::shared_ptr<message_filters::Synchronizer<message_filters::sync_policies::ApproximateTime<
    sensor_msgs::msg::Image, sensor_msgs::msg::Image>>> m_streams_sync; //!< Subscriber to synchronized RGB and depth streams when depth is used
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr m_rgb_only_stream_sub; //!< RGB stream subscriber when depth is not used

  // ----- Publisher -----

  // ----- Display-related attributes -----

  // ----- Tracking-related attributes -----
  bool m_depth_cam_info_received = false; //!< Set to true once the color camera parameters have been retrieved.
  std::string m_depth_camera_info_topic_name; //!< The name of the depth camera topic.
  std::string m_depth_stream_name; //!< The name of the depth image topic.
  VISP_NAMESPACE_ADDRESSING vpCameraParameters m_depth_cam; //!< The depth camera parameters.
  bool m_depth_is_required = false; //!< If true, it means that the tracker requires a depth stream to run.
  double m_z_factor; //!< Factor to translate uint16_t depth image into meters.
  std::string m_frame_id; //!< The frame ID of the RGB stream, in which will be expressed the tracking result.

  // ----- Other attributes -----

};
}
#endif
