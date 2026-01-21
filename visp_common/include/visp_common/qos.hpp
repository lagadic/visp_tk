/*
 * This file is part of the ViSP software.
 * Copyright (C) 2005 - 2026 by INRIA. All rights reserved.
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * ("GPL") version 2 as published by the Free Software Foundation.
 * See the file LICENSE.txt at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * For using ViSP with software that can not be combined with the GNU
 * GPL, please contact INRIA about acquiring a ViSP Professional
 * Edition License.
 *
 * See https://visp.inria.fr for more information.
 *
 * This software was developed at:
 * INRIA Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 * https://team.inria.fr/rainbow/
 *
 * If you have questions regarding the use of this file, please contact
 * INRIA at visp@inria.fr
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Contact visp@irisa.fr if any conditions of this licensing are
 * not clear to you.
 */

/*!
  \file qos.hpp
  \brief Conversions between strings and ROS QoS
*/

#ifndef VISP_COMMON__QOS_H
#define VISP_COMMON__QOS_H

#include <string>
#include <visp3/core/vpIoTools.h>

namespace visp_common
{
namespace qos
{
typedef enum STREAM_QOS_RELIABILITY
{
  QOS_RELIABILITY_RELIABLE = 0,
  QOS_RELIABILITY_BEST_EFFORT = 1,
  QOS_RELIABILITY_COUNT = 2,
  QOS_RELIABILITY_UNKNOWN = QOS_RELIABILITY_COUNT
}STREAM_QOS_RELIABILITY;

std::string reliabilityToString(const STREAM_QOS_RELIABILITY &type);

STREAM_QOS_RELIABILITY reliabilityFromString(const std::string &name);

std::string reliabilityList(const std::string &pref = "< ", const std::string sep = " , ", const std::string &suf = " >");

typedef enum STREAM_QOS_DURABILITY
{
  QOS_DURABILITY_VOLATILE = 0,
  QOS_DURABILITY_TRANSIENT = 1,
  QOS_DURABILITY_COUNT = 2,
  QOS_DURABILITY_UNKNOWN = QOS_DURABILITY_COUNT
}STREAM_QOS_DURABILITY;

std::string durabilityToString(const STREAM_QOS_DURABILITY &type);
STREAM_QOS_DURABILITY durabilityFromString(const std::string &name);
std::string durabilityList(const std::string &pref = "< ", const std::string sep = " , ", const std::string &suf = " >");
}
}

#endif
