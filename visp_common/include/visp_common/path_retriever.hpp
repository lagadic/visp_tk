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
  \file path_retriever.hpp
  \brief Conversions between ROS packages:// file:// and native filepath
*/

#ifndef VISP_COMMON__PATH_RETRIEVER_H_
#define VISP_COMMON__PATH_RETRIEVER_H_

namespace visp_common
{
namespace path
{
/*!
  \brief Converts a ROS `package://` and native filepath
  \param[in] path ROS filepath format
  \return native filepath format OR "" if could no parse filepath
*/
std::string path_retriever(const std::string path);
}
} // namespace visp_common

#endif // VISP_COMMON__PATH_RETRIEVER_H_
