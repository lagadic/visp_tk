cmake_minimum_required(VERSION 3.8)
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
find_package(VISP 3.7 REQUIRED COMPONENTS visp_core visp_ar visp_rbt OPTIONAL_COMPONENTS visp_gui)

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

ament_package()
