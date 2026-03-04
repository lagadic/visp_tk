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

#ifndef BASE_TRACKER_HPP
#define BASE_TRACKER_HPP

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <std_srvs/srv/trigger.hpp>

#include <tf2/exceptions.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>

#include <visp_common/camera.hpp>
#include <visp_common/path_retriever.hpp>
#include <visp_tracker_common/names.hpp>
#include <visp_tracker_common/msg/info_strings.hpp>
#include <visp_tracker_common/msg/named_feature_array.hpp>

#include <visp3/core/vpCameraParameters.h>

#include <optional>

#ifdef ENABLE_VISP_NAMESPACE
using namespace VISP_NAMESPACE_NAME;
#endif

namespace visp_tracker_common
{
class BaseTracker : public rclcpp::Node
{
public:
/**
   * @brief Constructor of the abstract base class.
   *
   * @param node_name The name of the node.
   * @param does_publish_features If headless mode is active, set to true if the node must publish 2D points to have a feedback on the remote GUI.
   */
  BaseTracker(const std::string &node_name, const bool &does_publish_features);

  virtual ~BaseTracker() = default;

  /**
   * @brief Initializes the node
   *
   * @return true The initialization went well.
   * @return false Otherwise.
   */
  virtual bool init();

  /**
   * @brief Check if the user asked to quit the node
   *
   * @return true The user wants to quit the node
   * @return false Otherwise
   */
  inline bool has_to_quit() const
  {
    return m_quit;
  }

  /**
   * @brief Method to stop the robot and quit the node.
   *
   */
  virtual void stop_and_quit();

protected:
  /** @name  Initialization */
  ///@{

  /**
   * @brief Initilize the tracker.
   *
   * @return true The initialization went well
   * @return false A problem occured
   */
  virtual bool init_tracker() = 0;

  /**
   * @brief Initialize the m_info_strings vector with constant strings to
   * give the user some info.
   */
  virtual void init_info_strings() = 0;

  /**
   * @brief Initialize the m_init_attribute from the associated ROS2 parameter.
   *
   * @param p The associated ROS2 parameter.
   * @return true The initialization was successful.
   * @return false Otherwise.
   */
  bool init_initialization_method(const rclcpp::Parameter &p);

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
  void color_camera_info_callback(const sensor_msgs::msg::CameraInfo::SharedPtr msg);

  /**
   * @brief Initial pose callback.
   *
   * @param msg Initial pose message
   */
  void init_pose_callback(const geometry_msgs::msg::PoseStamped::SharedPtr msg);

  // ----- Services -----

  /**
   * @brief Callback that is called to terminate the node.
   *
   * @param request Trigger signal.
   * @param response Response containing the status and the message.
   */
  void quit_callback(const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
                             std::shared_ptr<std_srvs::srv::Trigger::Response> response);

  /**
   * @brief Callback that is called to stop or start the tracking.
   *
   * @param request Trigger signal.
   * @param response Empty response.
   */
  void switch_tracking_status_callback(const  std::shared_ptr<std_srvs::srv::Trigger::Request> request,
                             std::shared_ptr<std_srvs::srv::Trigger::Response> response);

  /**
   * @brief Callback that is called to stop or start the visual feedback.
   *
   * @param request Trigger signal.
   * @param response Empty response.
   */
  void switch_visual_status_callback(const  std::shared_ptr<std_srvs::srv::Trigger::Request> request,
                             std::shared_ptr<std_srvs::srv::Trigger::Response> response);

  ///@}

  /**
   * @brief Enumeration that list the different ways of initializing the trackers.
   */
  typedef enum InitializationMethod
  {
    CLICK = 0, //!< The initialization of the tracker is done through user-interaction by clicking on particular points in the image. Requires visp_gui
    TOPIC = 1, //!< When the tracker is not initialized, it will listen on a dedicated topic to get the initial pose.
    FILE = 2, //!< When the tracker is not initialized, it will read the file that is given as node parameter.
    INITIALIZATION_METHOD_COUNT = 3 //!< Number of initilialization method.
  } InitializationMethod;

  /**
   * @brief Cast a BaseTracker::InitializationMethod into a string.
   *
   * @param method The method we want the name.
   * @return std::string The corresponding name.
   */
  static std::string initializationMethodToString(const InitializationMethod &method);

  /**
   * @brief Cast a string into a BaseTracker::InitializationMethod.
   *
   * @param name The name of the method.
   * @return InitializationMethod The corresponding enum value.
   */
  static InitializationMethod initializationMethodFromString(const std::string &name);

  /**
   * @brief Get the list of available InitializationMethod.
   *
   * @param prefix The prefix for the list.
   * @param sep The separator in the list.
   * @param suffix The suffix of the list.
   * @return std::string The list as a single string.
   */
  static std::string getAvailableInitializationMethod(const std::string &prefix = "< ", const std::string &sep = " , ", const std::string &suffix = " >");

  // ----- Parameters changes handling -----
  std::shared_ptr<rclcpp::ParameterEventHandler> param_subscriber_; //!< Suscriber to parameters changes.
  std::shared_ptr<rclcpp::ParameterCallbackHandle> cb_handle_; //!< Callback handler for dynamic change of the number of frames to skip when not in headless mode.
  std::shared_ptr<rclcpp::ParameterCallbackHandle> cb_handle_init_; //!< Callback handler for dynamic change of the initialization method.

  // ----- Services -----
  rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr m_quit_srv; //!< Service to quit the node using ros2 service
  rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr m_switch_tracking_status_srv; //!< Service to switch ON/OFF the tracking
  rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr m_switch_visual_srv; //!< Service to switch ON/OFF the visual debbuging

  // ----- Subscribers -----
  rclcpp::Subscription<sensor_msgs::msg::CameraInfo>::SharedPtr m_rgb_cam_info_sub; //!< RGB camera parameters subscriber
  rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr m_init_sub; //!< Subscriber to the init topic, if the init method is set to topic.
  std::shared_ptr<tf2_ros::TransformListener> m_tf_listener { nullptr }; //!< Listener in case the user wants to set the extrinsics from `tf2_static` topic
  std::unique_ptr<tf2_ros::Buffer> m_tf_buffer { nullptr }; //!< The buffer for the TF2

  // ----- Publisher -----
  rclcpp::Publisher<visp_tracker_common::msg::NamedFeatureArray>::SharedPtr m_features_pub; //!< 2D image points publisher, for remote GUI visualization when headless mode is active
  rclcpp::Publisher<visp_tracker_common::msg::InfoStrings>::SharedPtr m_info_strings_pub; //!< Publisher of the infos that are displayed on screen
  rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr m_poses_pub; //!< Poses publisher, for remote GUI visualization when headless mode is active

  // ----- Display-related attributes -----
  bool m_is_headless_mode = false; //!< True if the GUI is managed by another node
  int m_display_nb_frames_skipped = -1; //!< If positive, the display will be updated only one every m_display_nb_frames_skipped frames, otherwise the display is always updated.
  unsigned int m_frame_cnt = 0; //!< Counter for the display frame skip
  visp_tracker_common::msg::InfoStrings m_info_strings; //!< Vector that contains strings to display on string to give the user some info.
  unsigned int m_info_nb_static; //!< Number of info strings that must be visible all the time.
  bool m_visualization_debug = false; //!< Set to true if the node must publish 3D markers to visualize some points of interest in RVIZ

  // ----- Tracking-related attributes -----
  bool m_rgb_cam_info_received = false; //!< Set to true once the color camera parameters have been retrieved.
  std::string m_rgb_camera_topic_name; //!< The name of the color camera topic.
  std::string m_rgb_stream_name; //!< The name of the color image topic.
  vpCameraParameters m_rgb_cam; //!< The color camera parameters.
  std::string m_config_file; //!< If set, path to the configuration file that should be used to initialize the tracker.
  InitializationMethod m_init_method; //!< How the tracker should be initialized.
  std::optional<geometry_msgs::msg::PoseStamped> m_opt_init_pose; //!< If the init method is set to topic, this attribute is updated through the init topic.

  // ----- Other attributes -----
  std::mutex m_mutex_quit; //!< Mutex to protect m_quit from concurrent access
  bool m_quit = false; //!< Set to true when a SIGINT is received or when the user clicks on the screen
  std::mutex m_mutex_tracking; //!< Mutex to protect m_quit from concurrent access
  bool m_has_to_track = false; //!< Set to true when the tracker must run
  std::mutex m_mutex_visualization; //!< Mutex to protect m_visualization_debug from concurrent access

  static const std::string s_dumb_topic_name; //!< Dumb name to avoid error when a topic name has not been given. The error will be handled in the init() method.
  static const unsigned int s_default_hor_offset; //!< Default horizontal offset to display the m_info_strings information
};
}
#endif
