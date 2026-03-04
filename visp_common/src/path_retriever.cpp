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

/*!
  \file path_retriever
  \brief conversions between ROS packages:// file:// and native filepath
*/
#include <cstring>
#include <sstream>
#include <string>

#include <ament_index_cpp/get_package_prefix.hpp>
#include <ament_index_cpp/get_package_share_directory.hpp>

#include <visp_common/path_retriever.hpp>

using std::istringstream;

namespace visp_common
{
namespace path
{
std::string path_retriever(const std::string path)
{
  std::string retrieve_path;

  std::string line;
  std::istringstream sin;

  std::string mod_url = path;
  if (path.find("package://") == 0) {
    mod_url.erase(0, strlen("package://"));
    size_t pos = mod_url.find("/");
    if (pos == std::string::npos) {
      return "";
    }

    std::string package = mod_url.substr(0, pos);
    // delete package name
    mod_url.erase(0, pos);
    std::string package_path;
    try {
      package_path = ament_index_cpp::get_package_share_directory(package);
    }
    catch (const ament_index_cpp::PackageNotFoundError &) {
      return "";
    }
    mod_url = package_path + mod_url;
  }
  return mod_url;
}
}
} // namespace visp_common
