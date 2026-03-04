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

#ifndef NAMES_HPP
#define NAMES_HPP

#include <string>

namespace visp_tracker_common
{
extern const std::string quit_srv_name; //!< Name of the service to quit the demos
extern const std::string switch_tracking_srv_name; //!< Name of the service to activate / deactivate the tracking.
extern const std::string switch_vismode_srv_name; //!< Name of the service to activate / deactivate the visualization debug.

extern const std::string features2D_topic_name; //!< Name of the topic on which are published 2D features to display in the remote GUI.
extern const std::string info_strings_topic_name; //!< Name of the topic on which is published some messages to display on screen.
extern const std::string pose_topic_name; //!< Name of the topic on which are published the pose.
}
#endif
