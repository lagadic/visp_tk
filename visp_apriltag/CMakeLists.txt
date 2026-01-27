cmake_minimum_required(VERSION 3.8)
project(visp_apriltag)

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
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(message_filters REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(visp_common REQUIRED)
find_package(visp_tracker_common REQUIRED)
find_package(VISP REQUIRED COMPONENTS visp_core visp_detection OPTIONAL_COMPONENTS visp_gui)

add_executable(${PROJECT_NAME}_main
  src/april_tag_tracker.cpp
  src/AprilTagTracker.cpp
)

target_include_directories(${PROJECT_NAME}_main PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include/${PROJECT_NAME}>"
)

target_link_libraries(${PROJECT_NAME}_main
  ${VISP_LIBRARIES}
)

ament_target_dependencies(${PROJECT_NAME}_main
  rclcpp
  geometry_msgs
  message_filters
  sensor_msgs
  visp_common
  visp_tracker_common
)

install(
  TARGETS
  ${PROJECT_NAME}_main
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
