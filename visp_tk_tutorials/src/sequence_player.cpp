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

#include <chrono>

#include <rclcpp/rclcpp.hpp>
#include <visp_tk_tutorials/SequencePlayer.hpp>

int main(int argc, char *argv[])
{
  // rclcpp::init() installs the SIGINT/SIGTERM handling: on Ctrl-C, rclcpp::ok() becomes false.
  // A handler installed with signal() before rclcpp::init() would be replaced, and the loop below would then go on
  // spinning a shut down context, which crashed the node when stopping the launch file.
  rclcpp::init(argc, argv);

  bool status = false;
  {
    auto player = std::make_shared<visp_tk_tutorials::SequencePlayer>("player_rbt");
    status = player->init();
    if (status) {
      rclcpp::executors::SingleThreadedExecutor executor;
      executor.add_node(player);
      // spin_once() waits for the next event (e.g. the publication timer), whereas spin_some() returns immediately
      // when there is nothing to do, which made this loop use a whole CPU core
      while (rclcpp::ok() && !player->has_to_quit()) {
        executor.spin_once(std::chrono::milliseconds(30));
      }
      executor.remove_node(player);
    }
  }  // player destroyed here, while the context is still alive

  rclcpp::shutdown();
  return (status ? EXIT_SUCCESS : EXIT_FAILURE);
}
