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

#include <csignal>

#include <rclcpp/rclcpp.hpp>

#include <visp_tracker_common/TrackerGUI.hpp>

std::shared_ptr<visp_tracker_common::TrackerGUI> gui;

void signalHandler(int signum)
{
  // Send a quit request to the tracker and set the GUI state to quit
  if (gui) {
    gui->quit();
  }
  RCLCPP_WARN(gui->get_logger(), "Sent termination signal to the node due to signal %d", signum);
}

int main(int argc, char *argv[])
{
  signal(SIGABRT, signalHandler);
  signal(SIGINT, signalHandler);
  signal(SIGTERM, signalHandler);

  rclcpp::init(argc, argv);
  gui = std::make_shared<visp_tracker_common::TrackerGUI>("tracker_gui");
  auto it_node = std::make_shared<rclcpp::Node>("it_node");
  bool status = gui->init(it_node);
  if (status) {
    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(it_node);
    executor.add_node(gui);
    while (gui->has_to_run()) {
      executor.spin_some();
    }
  }

  if (it_node) {
    it_node.reset();
  }

  if (gui) {
    gui.reset();
  }
  rclcpp::shutdown();
  return (status ? EXIT_SUCCESS : EXIT_FAILURE);
}
