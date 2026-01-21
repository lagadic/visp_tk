cmake_minimum_required(VERSION 3.8)
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
find_package(vision_msgs REQUIRED)

# Find ViSP libraries and packages
find_package(VISP REQUIRED)
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
    "msg/NamedPose.msg"
    "msg/NamedPoseArray.msg"
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
# rosidl_target_interfaces(${PROJECT_NAME}_library
#   ${PROJECT_NAME} "rosidl_typesupport_cpp")

ament_target_dependencies(${PROJECT_NAME}_library
    ament_index_cpp
    geometry_msgs
    message_filters
    rclcpp
    sensor_msgs
    std_srvs
    visp_common
    VISP
  )

target_include_directories(${PROJECT_NAME}_library
  PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include/${PROJECT_NAME}>")

target_link_libraries(${PROJECT_NAME}_library
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

ament_export_targets(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)

ament_export_dependencies(
  ament_index_cpp
  geometry_msgs
  message_filters
  rclcpp
  rosidl_default_runtime
  sensor_msgs
  std_srvs
  visualization_msgs
  visp_common
  VISP
  # "${cpp_typesupport_target}"
)

# ##### #
# Nodes #
# ##### #

add_executable(tracker_gui src/tracker_gui.cpp src/TrackerGUI.cpp)

set_property(TARGET tracker_gui PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET tracker_gui PROPERTY CXX_STANDARD 17)

ament_target_dependencies(tracker_gui
    ament_index_cpp
    geometry_msgs
    image_transport
    rclcpp
    sensor_msgs
    std_srvs
    visp_common
    VISP
  )

target_link_libraries(tracker_gui
  ${VISP_LIBRARIES}
  ${PROJECT_NAME}_library
  "${cpp_typesupport_target}"
)

install(
  TARGETS
  tracker_gui
  DESTINATION lib/${PROJECT_NAME}
)

# ###### #
# Others #
# ###### #

# install(
#   DIRECTORY launch
#   DESTINATION share/${PROJECT_NAME}
# )

ament_package()
