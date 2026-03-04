cmake_minimum_required(VERSION 3.5)

project(visp_tk_tutorials)

find_package(ament_cmake REQUIRED)

#install(
#  DIRECTORY config launch
#  DESTINATION share/${PROJECT_NAME}
#)

# ############
# Bag files #
# ############
# include(ExternalProject)
# ExternalProject_Add(
#   external_bag
#   PREFIX "externals"
#   URL https://github.com/lagadic/vision_visp/releases/download/vision_visp-0.5.0/tutorial-qrcode-jazzy.tar.gz
#   DOWNLOAD_NO_EXTRACT true
#   CONFIGURE_COMMAND ""
#   PATCH_COMMAND ""
#   BUILD_COMMAND ""
#   INSTALL_COMMAND ""
# )

# ExternalProject_Get_Property(external_bag DOWNLOADED_FILE)

# set(BAG_DIR ${CMAKE_CURRENT_BINARY_DIR}/bag)
# set(BAG_METADATA ${BAG_DIR}/metadata.yaml)
# set(BAG_DB ${BAG_DIR}/tutorial-qrcode-jazzy.db3)

# add_custom_command(
#   OUTPUT ${BAG_METADATA} ${BAG_DB}
#   COMMAND ${CMAKE_COMMAND} -E make_directory ${BAG_DIR}
#   COMMAND tar -xzvf ${DOWNLOADED_FILE} -C ${BAG_DIR}
#   DEPENDS external_bag
#   COMMENT "Extracting ROS2 bag"
#   VERBATIM
# )

# add_custom_target(
#   extract_bag ALL
#   DEPENDS ${BAG_METADATA} ${BAG_DB}
#   VERBATIM
# )

# install(DIRECTORY
#   ${BAG_DIR}
#   DESTINATION share
# )


ament_package()
