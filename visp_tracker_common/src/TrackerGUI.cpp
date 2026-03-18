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

#include <visp_tracker_common/TrackerGUI.hpp>

namespace visp_tracker_common
{
TrackerGUI::TrackerGUI(const std::string &node_name)
  : rclcpp::Node(node_name)
{
  //////////////////////////////////////////////////////////////////////
  //                        ROS2 PARAMETERS                           //
  //////////////////////////////////////////////////////////////////////
  // // ---- Parameters related to the services ----

  // // ---- Parameters related to the publishers / subscribers ----
  this->declare_parameter<std::string>("image_transport", "compressed");
  this->declare_parameter<std::string>("camera_topic", "");
  this->declare_parameter<std::string>("color_topic", "");
  this->declare_parameter("color_qos_queue_depth", 1);
  this->declare_parameter("color_qos_durability", "volatile");
  this->declare_parameter("color_qos_reliability", "best_effort");
  this->declare_parameter("apriltag_topics", std::vector<std::string>());
  this->declare_parameter("features_topics", std::vector<std::string>());
  this->declare_parameter("poses_topics", std::vector<std::string>());
  this->declare_parameter("poses_names", std::vector<std::string>());
  this->declare_parameter("depth_topic", "");
  this->declare_parameter<std::string>("depth_image_transport", "compressedDepth");
  this->declare_parameter("depth_qos_queue_depth", 1);
  this->declare_parameter("depth_qos_durability", "volatile");
  this->declare_parameter("depth_qos_reliability", "best_effort");
  m_client_nodes_name = this->declare_parameter<std::vector<std::string>>("client_nodes", std::vector<std::string>());

  // // ---- Other parameters ----
  auto use_depth_desc = rcl_interfaces::msg::ParameterDescriptor {};
  use_depth_desc.description = "If true, the GUI will also subscribe to the compressed depth stream..";
  this->declare_parameter<bool>("use_depth", false, use_depth_desc);

  auto depth_scale_desc = rcl_interfaces::msg::ParameterDescriptor {};
  depth_scale_desc.description = "The depth scale that converts meters into raw depth image (uint16_t).";
  this->declare_parameter<double>("depth_scale", 1e3, depth_scale_desc);

  auto depth_min_desc = rcl_interfaces::msg::ParameterDescriptor {};
  depth_min_desc.description = "The minimum depth to display, in meters. If not set, any point closer than max_depth will be displayed.";
  this->declare_parameter<double>("min_depth", std::numeric_limits<double>::max(), depth_min_desc);

  auto depth_max_desc = rcl_interfaces::msg::ParameterDescriptor {};
  depth_max_desc.description = "The maximum depth to display, in meters. If not set, any point further than min_depth will be displayed.";
  this->declare_parameter<double>("max_depth", -1., depth_max_desc);

  auto features_thick_desc = rcl_interfaces::msg::ParameterDescriptor {};
  features_thick_desc.description = "Thickness to use to display the 2D features.";
  this->declare_parameter("features_thickness", 1, features_thick_desc);
  m_features_thickness = this->get_parameter("features_thickness").as_int();

  auto features_type_desc = rcl_interfaces::msg::ParameterDescriptor {};
  features_type_desc.description = "Available types of 2D visualization for the potential 2D features we are listening to: " + getAvailableFeaturesType();
  this->declare_parameter("features_type", "point", features_type_desc);
  m_features_type = featuresTypeFromString(this->get_parameter("features_type").as_string());


  //////////////////////////////////////////////////////////////////////
  //                        ROS2 SERVICES                             //
  //////////////////////////////////////////////////////////////////////
  m_switch_request = std::make_shared<std_srvs::srv::Trigger::Request>();
  m_quit_request = std::make_shared<std_srvs::srv::Trigger::Request>();
  rclcpp::NodeOptions options;
  m_service_node = rclcpp::Node::make_shared("tracker_gui_service_manager", options);

  //////////////////////////////////////////////////////////////////////
  //                        ROS2 PUB/SUB                              //
  //////////////////////////////////////////////////////////////////////

}

bool TrackerGUI::init(std::shared_ptr<rclcpp::Node> self)
{
  if (m_client_nodes_name.empty()) {
    RCLCPP_ERROR(this->get_logger(), "client_nodes parameter was not set");
    return false;
  }

  //////////////////////////////////////////////////////////////////////
  //                        ROS2 SERVICES                             //
  //////////////////////////////////////////////////////////////////////
  for (const auto &client_name: m_client_nodes_name) {
    std::string quit_srv_name = std::string("/") + client_name + visp_tracker_common::quit_srv_name;
    m_clients_quit.emplace_back(m_service_node->create_client<std_srvs::srv::Trigger>(quit_srv_name));
    while (!m_clients_quit.back()->wait_for_service(std::chrono::seconds(1))) {
      if (!rclcpp::ok()) {
        RCLCPP_ERROR(this->get_logger(), "Interrupted while waiting for the service. Exiting.");
        return false;
      }
      RCLCPP_INFO(this->get_logger(), "service %s not available, waiting again...", quit_srv_name.c_str());
    }
    RCLCPP_INFO(this->get_logger(), "service %s is now available", quit_srv_name.c_str());

    std::string switch_tracking_srv_name = std::string("/") + client_name + visp_tracker_common::switch_tracking_srv_name;
    m_clients_switch_tracking.emplace_back(m_service_node->create_client<std_srvs::srv::Trigger>(switch_tracking_srv_name));
    while (!m_clients_switch_tracking.back()->wait_for_service(std::chrono::seconds(1))) {
      if (!rclcpp::ok()) {
        RCLCPP_ERROR(this->get_logger(), "Interrupted while waiting for the service. Exiting.");
        return false;
      }
      RCLCPP_INFO(this->get_logger(), "service %s not available, waiting again...", switch_tracking_srv_name.c_str());
    }
    RCLCPP_INFO(this->get_logger(), "service %s is now available", switch_tracking_srv_name.c_str());

    std::string switch_visual_srv_name = std::string("/") + client_name + visp_tracker_common::switch_vismode_srv_name;
    m_clients_switch_visualization.emplace_back(m_service_node->create_client<std_srvs::srv::Trigger>(switch_visual_srv_name));
    while (!m_clients_switch_visualization.back()->wait_for_service(std::chrono::seconds(1))) {
      if (!rclcpp::ok()) {
        RCLCPP_ERROR(this->get_logger(), "Interrupted while waiting for the service. Exiting.");
        return false;
      }
      RCLCPP_INFO(this->get_logger(), "service %s not available, waiting again...", switch_visual_srv_name.c_str());
    }
    RCLCPP_INFO(this->get_logger(), "service %s is now available", switch_visual_srv_name.c_str());
  }

  //////////////////////////////////////////////////////////////////////
  //                        ROS2 PUB/SUB                              //
  //////////////////////////////////////////////////////////////////////

  // ---- RGB-related ----
  std::string rgb_topic_name = this->get_parameter("color_topic").as_string();
  if (rgb_topic_name.empty()) {
    RCLCPP_ERROR(this->get_logger(), "'color_topic' has not been set");
    return false;
  }

  std::string image_transport_type = this->get_parameter("image_transport").as_string();
  if (image_transport_type.empty()) {
    RCLCPP_ERROR(this->get_logger(), "'image_transport' has not been set");
    return false;
  }

  m_it_node = self;
  m_it = std::make_shared<image_transport::ImageTransport>(m_it_node);
  m_hints = std::make_shared<image_transport::TransportHints>(m_it_node.get(), image_transport_type);
  rmw_qos_profile_t compressed_color_qos = rmw_qos_profile_default;
  compressed_color_qos.depth = this->get_parameter("color_qos_queue_depth").as_int();
  compressed_color_qos.durability = rmw_qos_durability_policy_from_str(this->get_parameter("color_qos_durability").as_string().c_str());
  compressed_color_qos.reliability = rmw_qos_reliability_policy_from_str(this->get_parameter("color_qos_reliability").as_string().c_str());
  m_sub_color = m_it->subscribe(rgb_topic_name, compressed_color_qos, std::bind(&TrackerGUI::image_callback, this, std::placeholders::_1), image_transport::ImageTransport::VoidPtr(), m_hints.get(), rclcpp::SubscriptionOptions());

  std::string rgb_cam_topic = this->get_parameter("camera_topic").as_string();
  if (rgb_cam_topic.empty()) {
    RCLCPP_ERROR(this->get_logger(), "'camera_topic' has not been set");
    return false;
  }
  auto qos_cam_params = rclcpp::QoS(rclcpp::KeepLast(10)).best_effort().durability_volatile();
  RCLCPP_INFO(this->get_logger(), "Subscribing to camera topic '%s'", rgb_cam_topic.c_str());
  m_rgb_cam_sub = this->create_subscription<sensor_msgs::msg::CameraInfo>(rgb_cam_topic, qos_cam_params, std::bind(&TrackerGUI::camera_info_callback, this, std::placeholders::_1));

  // ---- Depth-related ----
  if (this->get_parameter("use_depth").as_bool()) {

    std::string depth_topic_name = this->get_parameter("depth_topic").as_string();
    if (depth_topic_name.empty()) {
      RCLCPP_ERROR(this->get_logger(), "'depth_topic' has not been set");
      return false;
    }
    rmw_qos_profile_t compressed_depth_qos = rmw_qos_profile_default;
    compressed_depth_qos.depth = this->get_parameter("depth_qos_queue_depth").as_int();
    compressed_depth_qos.durability = rmw_qos_durability_policy_from_str(this->get_parameter("depth_qos_durability").as_string().c_str());
    compressed_depth_qos.reliability = rmw_qos_reliability_policy_from_str(this->get_parameter("depth_qos_reliability").as_string().c_str());

    RCLCPP_INFO(this->get_logger(), "Subscribing to depth topic '%s'", depth_topic_name.c_str());
    m_hints_depth = std::make_shared<image_transport::TransportHints>(m_it_node.get(), this->get_parameter("depth_image_transport").as_string(), "depth_image_transport");
    m_sub_depth = m_it->subscribe(depth_topic_name, compressed_depth_qos, std::bind(&TrackerGUI::depth_callback, this, std::placeholders::_1), image_transport::ImageTransport::VoidPtr(), m_hints_depth.get(), rclcpp::SubscriptionOptions());

    const double depth_scale = this->get_parameter("depth_scale").as_double();
    double min_depth = this->get_parameter("min_depth").as_double();
    if (min_depth < 50.) {
      m_opt_min_depth = static_cast<uint16_t>(min_depth * depth_scale);
    }
    double max_depth = this->get_parameter("max_depth").as_double();
    if (max_depth > 0.) {
      m_opt_max_depth = static_cast<uint16_t>(max_depth * depth_scale);
    }
  }

  // ---- info/visual-features -related
  auto qos_infostr_pub = rclcpp::QoS(rclcpp::KeepLast(5)).best_effort().transient_local();
  std::string info_srv_name = std::string("/") + m_client_nodes_name[0] + visp_tracker_common::info_strings_topic_name;
  m_info_strings_sub = this->create_subscription<visp_tracker_common::msg::InfoStrings>(info_srv_name, qos_infostr_pub, std::bind(&TrackerGUI::info_callback, this, std::placeholders::_1));
  RCLCPP_INFO(this->get_logger(), "Subscribed to info strings topic '%s'", m_info_strings_sub->get_topic_name());

  auto qos_display = rclcpp::QoS(rclcpp::KeepLast(10)).best_effort().durability_volatile();
  std::vector<std::string> features_topic_name = this->get_parameter("features_topics").as_string_array();
  if (!features_topic_name.empty()) {
    const unsigned int nb_topics = features_topic_name.size();
    m_feature_opt_vec.resize(nb_topics, std::nullopt);
    for (unsigned int i = 0; i < nb_topics; ++i) {
      RCLCPP_INFO(this->get_logger(), "Subscribing to features topic '%s'", features_topic_name[i].c_str());
      m_feat_2D_sub.emplace_back(this->create_subscription<visp_tracker_common::msg::NamedFeatureArray>(features_topic_name[i], qos_display, [this, i](const visp_tracker_common::msg::NamedFeatureArray::ConstSharedPtr msg)
                                                                                                        {
                                                                                                          std::scoped_lock sl(m_mutex_features);
                                                                                                          m_feature_opt_vec[i] = std::move(*msg);
                                                                                                        }));
    }
  }

  std::vector<std::string> poses_topics_names = this->get_parameter("poses_topics").as_string_array();
  m_pose_name_array = this->get_parameter("poses_names").as_string_array();
  if (poses_topics_names.size() != m_pose_name_array.size()) {
    RCLCPP_ERROR_STREAM(this->get_logger(), "'poses_names' and 'poses_topics' do not contain the same number of items (" << poses_topics_names.size() << " vs " << m_pose_name_array.size() << ")");
    return false;
  }
  if (!poses_topics_names.empty()) {
    const unsigned int nb_topics = poses_topics_names.size();
    m_pose_opt_vec.resize(nb_topics, std::nullopt);
    for (unsigned int i = 0; i < nb_topics; ++i) {
      RCLCPP_INFO(this->get_logger(), "Subscribing to poses topic '%s'", poses_topics_names[i].c_str());
      m_poses_sub.emplace_back(this->create_subscription<geometry_msgs::msg::PoseStamped>(poses_topics_names[i], qos_display, [this, i](const geometry_msgs::msg::PoseStamped::ConstSharedPtr msg)
                                                                                          {
                                                                                            std::scoped_lock sl(m_mutex_poses);
                                                                                            m_pose_opt_vec[i] = std::move(*msg);
                                                                                          }));
    }
  }

  std::vector<std::string> tag_topics_names = this->get_parameter("apriltag_topics").as_string_array();
  if (!tag_topics_names.empty()) {
    auto qos_tag_info_pub = rclcpp::QoS(rclcpp::KeepLast(5)).best_effort().transient_local();
    const unsigned int nb_topics = tag_topics_names.size();
    m_apriltag_opt_vec.resize(nb_topics, std::nullopt);
    for (unsigned int i = 0; i < nb_topics; ++i) {
      RCLCPP_INFO(this->get_logger(), "Subscribing to AprilTag topic '%s'", tag_topics_names[i].c_str());
      m_apriltag_sub.emplace_back(this->create_subscription<visp_tracker_common::msg::AprilTagDetectionArray>(tag_topics_names[i], qos_tag_info_pub, [this, i](const visp_tracker_common::msg::AprilTagDetectionArray::ConstSharedPtr msg)
                                                                                                              {
                                                                                                                std::scoped_lock sl(m_mutex_apriltag);
                                                                                                                m_apriltag_opt_vec[i] = std::move(*msg);
                                                                                                              }));
    }
  }
  return true;
}

void TrackerGUI::quit()
{
  for (auto &client_quit: m_clients_quit) {
    auto result = client_quit->async_send_request(m_quit_request);
    RCLCPP_INFO(this->get_logger(), "Sent a quit request...");
    // Wait for the result.
    auto status = rclcpp::spin_until_future_complete(m_service_node, result, std::chrono::seconds(1));
    if (status == rclcpp::FutureReturnCode::SUCCESS) {
      RCLCPP_INFO(this->get_logger(), "Got a response !");
      auto response = result.get();
      RCLCPP_INFO(this->get_logger(), "Message : '%s'", response->message.c_str());
      if (!response->success) {
        RCLCPP_INFO(this->get_logger(), "Stop visualization debug ...");
      }
    }
    else if (status == rclcpp::FutureReturnCode::TIMEOUT) {
      client_quit->remove_pending_request(result);
      RCLCPP_ERROR(this->get_logger(), "Calling the quit service resulted in a timeout.");
    }
    else {
      RCLCPP_ERROR(this->get_logger(), "Failed to call switch service");
    }
  }
  std::scoped_lock sl(m_mutex_run);
  m_run = false;
  m_it.reset();
  m_hints.reset();
  m_hints_depth.reset();
  m_it_node.reset();
}

void TrackerGUI::switch_tracking()
{
  for (auto &client_switch_tracking: m_clients_switch_tracking) {
    auto result = client_switch_tracking->async_send_request(m_switch_request);
    RCLCPP_INFO(this->get_logger(), "Sent a switch tracking request...");
    // Wait for the result.
    if (rclcpp::spin_until_future_complete(m_service_node, result) == rclcpp::FutureReturnCode::SUCCESS) {
      RCLCPP_INFO(this->get_logger(), "Got a response !");
      auto response = result.get();
      RCLCPP_INFO(this->get_logger(), "Message : '%s'", response->message.c_str());
      if (!response->success) {
        RCLCPP_INFO(this->get_logger(), "Stopping tracking");
      }
    }
    else {
      RCLCPP_ERROR(this->get_logger(), "Failed to call switch service");
    }
  }
}

void TrackerGUI::switch_visualization()
{
  for (auto &client_switch_visualization: m_clients_switch_visualization) {
    auto result = client_switch_visualization->async_send_request(m_switch_request);
    RCLCPP_INFO(this->get_logger(), "Sent a switch visualization request...");
    // Wait for the result.
    if (rclcpp::spin_until_future_complete(m_service_node, result) == rclcpp::FutureReturnCode::SUCCESS) {
      RCLCPP_INFO(this->get_logger(), "Got a response !");
      auto response = result.get();
      RCLCPP_INFO(this->get_logger(), "Message : '%s'", response->message.c_str());
      if (!response->success) {
        RCLCPP_INFO(this->get_logger(), "Stop visualization debug ...");
      }
    }
    else {
      RCLCPP_ERROR(this->get_logger(), "Failed to call switch service");
    }
  }
}
//////////////////////////////////////////////////////////////////////
//                        ROS2 SUBCRIPTIONS                         //
//////////////////////////////////////////////////////////////////////
void TrackerGUI::camera_info_callback(const sensor_msgs::msg::CameraInfo::SharedPtr msg)
{
  if (m_opt_rgb_cam) {
    return;
  }

  m_opt_rgb_cam = visp_common::camera::toVispCameraParameters(*msg);
  m_rgb_cam_sub.reset(); // Remove the subscription to avoid unecessary interruptions

  RCLCPP_INFO_STREAM(this->get_logger(), "RGB camera intrinsics :" << std::endl << *m_opt_rgb_cam);
}

void TrackerGUI::image_callback(const sensor_msgs::msg::Image::ConstSharedPtr &msg)
{
  static const unsigned int left_hor_offset = 20;
  static const unsigned int v_offset = 20;

  bool run = false;
  {
    std::scoped_lock sl(m_mutex_run);
    run = m_run;
  }
  if (!run) {
    return;
  }

  // Converting RGB image
  m_I = std::move(visp_common::image::toVispImageRGBa(*msg));

  if (!m_display_color) {
    m_display_color = vpDisplayFactory::createDisplay(m_I, -1, -1, "Tracker GUI: color image");
  }

  // Getting depth image, if available
  static std::optional<vpImage<vpRGBa>> opt_Id = std::nullopt;
  if (m_opt_Id) {
    {
      std::scoped_lock sl(m_mutex_Id);
      opt_Id = m_opt_Id.value();
    }
  }
  if (opt_Id) {
    if (!m_display_depth) {
      m_display_depth = vpDisplayFactory::createDisplay(opt_Id.value(), m_display_color->getWidth() + 20, -1, "Remote GUI: depth image");
    }
  }

  bool display_frame = (m_display_nb_frames_skipped <= 0) || ((m_frame_cnt % m_display_nb_frames_skipped) == 0);
  ++m_frame_cnt;
  if (display_frame) {
    vpDisplay::display(m_I);
    if (opt_Id) {
      vpDisplay::display(opt_Id.value());
    }

    // Displaying how to use the GUI
    vpDisplay::displayText(m_I, v_offset, left_hor_offset, "Left click to turn ON/OFF the tracking.", vpColor::red);
    vpDisplay::displayText(m_I, v_offset * 2, left_hor_offset, "Middle click to turn ON/OFF the visual debug.", vpColor::red);
    vpDisplay::displayText(m_I, v_offset * 3, left_hor_offset, "Right click to stop the tracker and quit the GUI.", vpColor::red);

    // Displaying text info from the tracker
    {
      std::scoped_lock sl(m_mutex_info);
      for (unsigned int r = 0; r < m_vec_info.info_strings.size(); ++r) {
        vpDisplay::displayText(m_I, v_offset * (r + 1), m_I.getWidth() - m_vec_info.hor_offset_right_border[r], m_vec_info.info_strings[r], vpColor::red);
      }
    }

    // Displaying AprilTag detection, if any
    {
      std::scoped_lock sl(m_mutex_apriltag);
      for (auto &opt_apriltag_detection: m_apriltag_opt_vec) {
        if (opt_apriltag_detection) {
          for (const auto &detection: opt_apriltag_detection->detections) {
            if (m_opt_rgb_cam) {
              vpHomogeneousMatrix H = visp_common::pose::toVispHomogeneousMatrix(detection.pose.pose);
              vpDisplay::displayFrame(m_I, H, m_opt_rgb_cam.value(), detection.size / 2.f, vpColor::none, 2, vpImagePoint(0, 0), "ID: " + std::to_string(detection.id), vpColor::red);
            }
            for (unsigned int i = 0; i < detection.corners.size() - 1; ++i) {
              vpDisplay::displayLine(m_I, vpImagePoint(detection.corners[i].y, detection.corners[i].x), vpImagePoint(detection.corners[i + 1].y, detection.corners[i + 1].x), vpColor::blue, m_features_thickness);
            }
            vpDisplay::displayLine(m_I, vpImagePoint(detection.corners[3].y, detection.corners[3].x), vpImagePoint(detection.corners[0].y, detection.corners[0].x), vpColor::blue, m_features_thickness);
          }
        }
        opt_apriltag_detection = std::nullopt;
      }
    }

    // Displaying poses, if any
    {
      std::scoped_lock sl(m_mutex_poses);
      if (m_opt_rgb_cam) {
        unsigned int id = 0;
        for (auto &opt_pose: m_pose_opt_vec) {
          if (opt_pose) {
            vpHomogeneousMatrix H = visp_common::pose::toVispHomogeneousMatrix(opt_pose->pose);
            vpDisplay::displayFrame(m_I, H, m_opt_rgb_cam.value(), 0.03, vpColor::none, 2, vpImagePoint(0, 0), m_pose_name_array[id], vpColor::red);
          }
          opt_pose = std::nullopt;
          ++id;
        }
      }
    }

    // Displaying 2D features, if any
    {
      std::scoped_lock sl(m_mutex_features);
      int idx = 0;
      for (auto &feature_array: m_feature_opt_vec) {
        if (!feature_array) {
          continue;
        }
        for (const auto &named_feature: feature_array->features) {
          vpColor color = vpColor::allColors[idx % vpColor::nbColors];
          vpDisplay::displayText(m_I, m_display_color->getHeight() -  v_offset * (idx + 1), left_hor_offset, named_feature.name, color);

          // Displaying 2D points
          switch (m_features_type) {
          case FeaturesType::CROSS:
          {
            for (const auto &ip: named_feature.image_points) {
              vpDisplay::displayCross(m_I, ip.y, ip.x, 10, color, m_features_thickness);
            }
            break;
          }
          case FeaturesType::POINT:
          {
            for (const auto &ip: named_feature.image_points) {
              vpDisplay::displayPoint(m_I, ip.y, ip.x, color, m_features_thickness);
            }
            break;
          }
          default:
            RCLCPP_WARN_STREAM(this->get_logger(), "Visualization type for the 2D features is unknown, available types are: " << getAvailableFeaturesType());
          }

          // Displaying ellipses
          for (const auto &ellipse: named_feature.ellipses) {
            vpDisplay::displayEllipse(m_I, vpImagePoint(ellipse.center.y, ellipse.center.x), ellipse.n20, ellipse.n11, ellipse.n02, true, color, m_features_thickness);
          }

          // Displaying lines
          for (const auto &line: named_feature.lines) {
            vpDisplay::displayLine(m_I, vpImagePoint(line.start.y, line.start.x), vpImagePoint(line.end.y, line.end.x), color, m_features_thickness);
          }

          // Displaying polygons
          for (const auto &poly: named_feature.polygons) {
            std::vector<vpImagePoint> poly_points;
            poly_points.push_back(vpImagePoint(poly.lines[0].start.y, poly.lines[0].start.x));
            // Displaying each line of the polygon
            for (const auto &line: poly.lines) {
              poly_points.push_back(vpImagePoint(line.end.y, line.end.x));
            }
            vpDisplay::displayPolygon(m_I, poly_points, color, m_features_thickness);
          }

          // Displaying rectangles
          for (const auto &rect: named_feature.rectangles) {
            vpDisplay::displayRectangle(m_I, vpImagePoint(rect.start.y, rect.start.x), vpImagePoint(rect.end.y, rect.end.x), color, false, m_features_thickness);
          }

          ++idx;
        }
        feature_array = std::nullopt;
      }
    }

    vpDisplay::flush(m_I);
    if (opt_Id) {
      vpDisplay::flush(opt_Id.value());
    }
  }

  // Getting user interaction feedback
  vpMouseButton::vpMouseButtonType button;
  if (vpDisplay::getClick(m_I, button, false)) {
    switch (button) {
    case vpMouseButton::button1:
    {
      switch_tracking();
      break;
    }
    case vpMouseButton::button2:
    {
      switch_visualization();
      break;
    }
    case vpMouseButton::button3:
    {
      quit();
      break;
    }
    default:
      break;
    }
  }
}


void TrackerGUI::depth_callback(const sensor_msgs::msg::Image::ConstSharedPtr &msg)
{
  static const unsigned int height = msg->height;
  static const unsigned int width = msg->width;
  static const unsigned int size = height * width;
  static vpImage<uint16_t> Iuint16(msg->height, msg->width);

  Iuint16 = std::move(visp_common::image::toVispImageUint16(*msg));

  uint16_t min = std::numeric_limits<uint16_t>::max();
  uint16_t max = 0;
  if (!(m_opt_min_depth && m_opt_max_depth)) {
#ifdef VISP_HAVE_OPENMP
#pragma omp parallel for
#endif
    for (unsigned int idx = 0; idx < size; ++idx) {
      if ((Iuint16.bitmap[idx]!= std::numeric_limits<uint16_t>::max()) && (Iuint16.bitmap[idx] != 0)) {
        min = std::min(Iuint16.bitmap[idx], min);
        max = std::max(Iuint16.bitmap[idx], max);
      }
    }
  }

  if (m_opt_min_depth) {
    min = *m_opt_min_depth;
  }

  if (m_opt_max_depth) {
    max = *m_opt_max_depth;
  }

  float a = 255.f / static_cast<float>(max - min);
  float b = 255.f - a * static_cast<float>(max);

  vpImage<vpRGBa> Id(height, width, vpRGBa(0, 0, 0));
#ifdef VISP_HAVE_OPENMP
#pragma omp parallel for
#endif
  for (unsigned int idx = 0; idx < size; ++idx) {
    if (Iuint16.bitmap[idx] <= min) {
      Id.bitmap[idx].B = Id.bitmap[idx].R = Id.bitmap[idx].G = 255;
    }
    else if (Iuint16.bitmap[idx] >= max) {
      Id.bitmap[idx].B = Id.bitmap[idx].R = Id.bitmap[idx].G = 0;
    }
    else {
      Id.bitmap[idx].B = static_cast<unsigned char>(a * static_cast<float>(Iuint16.bitmap[idx]) + b);
      Id.bitmap[idx].R = 255 - Id.bitmap[idx].B;
    }
  }

  {
    std::scoped_lock sl(m_mutex_Id);
    m_opt_Id = Id;
  }
}

void TrackerGUI::info_callback(const visp_tracker_common::msg::InfoStrings::ConstSharedPtr msg)
{
  std::scoped_lock sl(m_mutex_info);
  m_vec_info = std::move(*msg);
}

//////////////////////////////////////////////////////////////////////
//                        OTHERS                                    //
//////////////////////////////////////////////////////////////////////
std::string TrackerGUI::featuresTypeToString(const TrackerGUI::FeaturesType &mode)
{
  switch (mode) {
  case POINT:
    return "point";
  case CROSS:
    return "cross";
  default:
    break;
  }
  return "unknown";
}

TrackerGUI::FeaturesType TrackerGUI::featuresTypeFromString(const std::string &name)
{
  FeaturesType res = FeaturesType::TYPE_COUNT;
  bool wasFound = false;
  std::string lowerCaseName = vpIoTools::toLowerCase(name);
  unsigned int i = 0;
  while ((i < FeaturesType::TYPE_COUNT) && (!wasFound)) {
    FeaturesType candidate = (FeaturesType)i;
    if (lowerCaseName == featuresTypeToString(candidate)) {
      res = candidate;
      wasFound = true;
    }
    ++i;
  }
  return res;
}

std::string TrackerGUI::getAvailableFeaturesType(const std::string &prefix, const std::string &sep, const std::string &suffix)
{
  std::string modes(prefix);
  for (unsigned int i = 0; i < FeaturesType::TYPE_COUNT - 1; ++i) {
    FeaturesType candidate = (FeaturesType)i;
    modes += featuresTypeToString(candidate) + sep;
  }
  FeaturesType candidate = (FeaturesType)(FeaturesType::TYPE_COUNT - 1);
  modes += featuresTypeToString(candidate) + suffix;
  return modes;
}
}
