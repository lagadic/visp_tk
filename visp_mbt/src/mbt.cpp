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

#include <rclcpp/rclcpp.hpp>
#include <visp_mbt/MBTTracker.hpp>

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);  // installs SIGINT/SIGTERM handling

  int ret = EXIT_FAILURE;
  {
    auto tracker = std::make_shared<visp_mbt::MBTTracker>("tracker_mbt");
    if (tracker->init()) {
      rclcpp::executors::SingleThreadedExecutor executor;
      executor.add_node(tracker);
      while (rclcpp::ok() && !tracker->has_to_quit()) {
        executor.spin_once(std::chrono::milliseconds(30));
      }
      executor.remove_node(tracker);
      ret = EXIT_SUCCESS;
    }
  }  // tracker destroyed here, while the context is still alive

  rclcpp::shutdown();
  return ret;
}
