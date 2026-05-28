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
project(visp_tracker_common)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(image_transport REQUIRED)
find_package(message_filters REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(vision_msgs REQUIRED)

# Find ViSP libraries and packages
find_package(VISP 3.7 REQUIRED COMPONENTS visp_core OPTIONAL_COMPONENTS visp_gui)
find_package(visp_common REQUIRED)

# ############## #
# Msg generation #
# ############## #
rosidl_generate_interfaces(${PROJECT_NAME}
    "msg/AprilTagDetection.msg"
    "msg/AprilTagDetectionArray.msg"
    "msg/Ellipse2D.msg"
    "msg/InfoStrings.msg"
    "msg/NamedFeature.msg"
    "msg/NamedFeatureArray.msg"
    "msg/Point2DTuple.msg"
    "msg/Polygon2D.msg"
    DEPENDENCIES geometry_msgs std_msgs vision_msgs # Add packages that above messages depend on
  )

rosidl_get_typesupport_target(cpp_typesupport_target "${PROJECT_NAME}" "rosidl_typesupport_cpp")

ament_export_dependencies(rosidl_default_runtime)

# ############
# Libraries #
# ############
add_library(${PROJECT_NAME}_library src/names.cpp src/BaseTracker.cpp src/BaseMultiModalTracker.cpp)
add_library(${PROJECT_NAME}::${PROJECT_NAME}_library ALIAS ${PROJECT_NAME}_library)

target_include_directories(${PROJECT_NAME}_library PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include/${PROJECT_NAME}>"
    ${VISP_INCLUDE_DIRS}
  )

target_link_libraries(${PROJECT_NAME}_library PUBLIC
    ${ament_index_cpp_TARGETS}
    ${geometry_msgs_TARGETS}
    ${message_filters_TARGETS}
    ${rclcpp_TARGETS}
    ${sensor_msgs_TARGETS}
    ${std_srvs_TARGETS}
    ${tf2_TARGETS}
    ${tf2_ros_TARGETS}
    ${visp_common_TARGETS}
    ${VISP_LIBRARIES}
    "${cpp_typesupport_target}"
  )

install(
  DIRECTORY include/
  DESTINATION include/${PROJECT_NAME}
)

install(
  TARGETS ${PROJECT_NAME}_library
  EXPORT export_${PROJECT_NAME}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)

install(EXPORT export_${PROJECT_NAME}
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION share/${PROJECT_NAME}/cmake
)

ament_export_include_directories(
  include
)

ament_export_targets(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)

ament_export_dependencies(
  ament_index_cpp
  geometry_msgs
  message_filters
  rclcpp
  rosidl_default_runtime
  sensor_msgs
  std_srvs
  tf2
  tf2_ros
  visualization_msgs
  visp_common
  VISP
  # "${cpp_typesupport_target}"
)

# ##### #
# Nodes #
# ##### #
if(VISP_GUI_FOUND)
  add_executable(tracker_gui src/tracker_gui.cpp src/TrackerGUI.cpp)

  set_property(TARGET tracker_gui PROPERTY CXX_STANDARD_REQUIRED ON)
  set_property(TARGET tracker_gui PROPERTY CXX_STANDARD 17)

  target_include_directories(tracker_gui PUBLIC include)
  target_include_directories(tracker_gui PUBLIC SYSTEM ${VISP_INCLUDE_DIRS})
  target_link_libraries(tracker_gui PUBLIC
      ${ament_index_cpp_TARGETS}
      ${geometry_msgs_TARGETS}
      ${image_transport_TARGETS}
      ${rclcpp_TARGETS}
      ${sensor_msgs_TARGETS}
      ${std_srvs_TARGETS}
      ${visp_common_TARGETS}
      ${VISP_LIBRARIES}
      ${PROJECT_NAME}_library
      "${cpp_typesupport_target}"
    )

  install(
    TARGETS
    tracker_gui
    DESTINATION lib/${PROJECT_NAME}
  )
else()
  message(WARNING "The module visp_gui has not been found, the tracker GUI will not be built.")
endif()


# ###### #
# Others #
# ###### #

# install(
#   DIRECTORY launch
#   DESTINATION share/${PROJECT_NAME}
# )

ament_package()
