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

project(visp_tk_tutorials)

find_package(ament_cmake REQUIRED)

install(
 DIRECTORY config launch
 DESTINATION share/${PROJECT_NAME}
)

# ############
# Bag files #
# ############
include(ExternalProject)
ExternalProject_Add(
  external_bag
  PREFIX "externals"
  URL https://visp-doc.inria.fr/download/visp_tk/tutorial-static-box-humble.tar.gz
  DOWNLOAD_NO_EXTRACT true
  CONFIGURE_COMMAND ""
  PATCH_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
)

ExternalProject_Get_Property(external_bag DOWNLOADED_FILE)

set(BAG_DIR ${CMAKE_CURRENT_BINARY_DIR}/bag)
set(BAG_MBT_SUBDIR mbt)
set(BAG_METADATA ${BAG_DIR}/${BAG_MBT_SUBDIR}/metadata.yaml)
set(BAG_DB ${BAG_DIR}/${BAG_MBT_SUBDIR}/tutorial-static-box-humble.db3)

add_custom_command(
  OUTPUT ${BAG_METADATA} ${BAG_DB}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${BAG_DIR}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${BAG_DIR}/${BAG_MBT_SUBDIR}
  COMMAND tar -xzvf ${DOWNLOADED_FILE} -C ${BAG_DIR}/${BAG_MBT_SUBDIR}
  DEPENDS external_bag
  COMMENT "Extracting ROS2 bag"
  VERBATIM
)

add_custom_target(
  extract_bag ALL
  DEPENDS ${BAG_METADATA} ${BAG_DB}
  VERBATIM
)

ExternalProject_Add(
  external_bag_apriltag
  PREFIX "externals"
  URL https://visp-doc.inria.fr/download/visp_tk/humble_short_apriltag_tutorial.tar.gz
  DOWNLOAD_NO_EXTRACT true
  CONFIGURE_COMMAND ""
  PATCH_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
)

ExternalProject_Get_Property(external_bag_apriltag DOWNLOADED_FILE)

set(BAG_APRILTAG_SUBDIR apriltag)
set(APRILTAG_BAG_METADATA ${BAG_DIR}/${BAG_APRILTAG_SUBDIR}/metadata.yaml)
set(APRILTAG_BAG_DB ${BAG_DIR}/${BAG_APRILTAG_SUBDIR}/humble_short_apriltag_tutorial.db3)

add_custom_command(
  OUTPUT ${APRILTAG_BAG_METADATA} ${APRILTAG_BAG_DB}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${BAG_DIR}/${BAG_APRILTAG_SUBDIR}
  COMMAND tar -xzvf ${DOWNLOADED_FILE} -C ${BAG_DIR}/${BAG_APRILTAG_SUBDIR}
  DEPENDS external_bag_apriltag
  COMMENT "Extracting ROS2 bag"
  VERBATIM
)

add_custom_target(
  extract_bag_apriltag ALL
  DEPENDS ${APRILTAG_BAG_METADATA} ${APRILTAG_BAG_DB}
  VERBATIM
)

install(DIRECTORY
  ${BAG_DIR}
  DESTINATION share/${PROJECT_NAME}
)


ament_package()
