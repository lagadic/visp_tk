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
#include <visp_rbt/RBTTracker.hpp>

std::shared_ptr<visp_rbt::RBTTracker> tracker;

void signalHandler(int signum)
{
  // Send a quit request to the tracker
  if (tracker) {
    tracker->stop_and_quit();
  }
  RCLCPP_WARN(tracker->get_logger(), "Sent termination signal to the node due to signal %d", signum);
}

int main(int argc, char *argv[])
{
  signal(SIGABRT, signalHandler);
  signal(SIGINT, signalHandler);
  signal(SIGTERM, signalHandler);

  rclcpp::init(argc, argv);
  tracker = std::make_shared<visp_rbt::RBTTracker>("tracker_rbt");
  bool status = tracker->init();
  if (status) {
    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(tracker);
    while (!tracker->has_to_quit()) {
      executor.spin_some();
    }
  }
  tracker.reset();
  rclcpp::shutdown();
  return (status ? EXIT_SUCCESS : EXIT_FAILURE);
}
