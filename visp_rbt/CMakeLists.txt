#############################################################################
#
# Copyright (C) 2026 by Inria. All rights reserved.
#
# This software is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# See the file LICENSE.txt at the root directory of this source
# distribution for additional information about the GNU GPL.
#
# For using this software that can not be combined with the GNU
# GPL, or if you have questions regarding the use of this file,
# please contact Inria at visp@inria.fr
#
# This software was developed at:
# Inria Rennes - Bretagne Atlantique
# Campus Universitaire de Beaulieu
# 35042 Rennes Cedex
# France
#
# This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
# WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
#
#############################################################################

cmake_minimum_required(VERSION 3.20)
project(visp_rbt)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


find_package(ament_cmake_auto REQUIRED)
ament_auto_find_build_dependencies()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(message_filters REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(visp_common REQUIRED)
find_package(visp_tracker_common REQUIRED)
find_package(VISP 3.7 REQUIRED COMPONENTS visp_core visp_ar OPTIONAL_COMPONENTS visp_rbt visp_gui)

if(VISP_RBT_FOUND)
  add_executable(${PROJECT_NAME}_node
    src/rbt.cpp
    src/RBTTracker.cpp
  )

  target_include_directories(${PROJECT_NAME}_node PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include/${PROJECT_NAME}>"
  )

target_link_libraries(${PROJECT_NAME}_node PUBLIC
  ${geometry_msgs_TARGETS}
  ${message_filters_TARGETS}
  ${rclcpp_TARGETS}
  ${sensor_msgs_TARGETS}
  ${visp_common_TARGETS}
  ${visp_tracker_common_TARGETS}
  ${VISP_LIBRARIES}
)

install(
  TARGETS
  ${PROJECT_NAME}_node
  DESTINATION lib/${PROJECT_NAME}
)

endif()
ament_package()
