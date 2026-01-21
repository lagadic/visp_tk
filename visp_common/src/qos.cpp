/*
 * This file is part of the ViSP software.
 * Copyright (C) 2005 - 2022 by INRIA. All rights reserved.
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
  \file qos.cpp
  \brief conversions between strings and ROS QoS
*/

#include <visp_common/qos.hpp>

namespace visp_common
{
namespace qos
{
std::string reliabilityToString(const STREAM_QOS_RELIABILITY &type)
{
  switch (type) {
  case QOS_RELIABILITY_RELIABLE:
    return "reliable";
  case QOS_RELIABILITY_BEST_EFFORT:
    return "best_effort";
  default:
    break;
  }
  return "unknown";
}

STREAM_QOS_RELIABILITY reliabilityFromString(const std::string &name)
{
  for (unsigned char idx = 0; idx < QOS_RELIABILITY_COUNT; ++idx) {
    STREAM_QOS_RELIABILITY temp = static_cast<STREAM_QOS_RELIABILITY>(idx);
    if (vpIoTools::toLowerCase(name) == reliabilityToString(temp)) {
      return temp;
    }
  }
  return QOS_RELIABILITY_UNKNOWN;
}

std::string reliabilityList(const std::string &pref, const std::string sep, const std::string &suf)
{
  std::string list = pref;
  for (unsigned char idx = 0; idx < QOS_RELIABILITY_COUNT - 1; ++idx) {
    STREAM_QOS_RELIABILITY temp = static_cast<STREAM_QOS_RELIABILITY>(idx);
    list += reliabilityToString(temp);
    list += sep;
  }
  list += reliabilityToString(static_cast<STREAM_QOS_RELIABILITY>(QOS_RELIABILITY_COUNT - 1));
  list += suf;
  return list;
}

std::string durabilityToString(const STREAM_QOS_DURABILITY &type)
{
  switch (type) {
  case QOS_DURABILITY_VOLATILE:
    return "volatile";
  case QOS_DURABILITY_TRANSIENT:
    return "transient";
  default:
    break;
  }
  return "unknown";
}

STREAM_QOS_DURABILITY durabilityFromString(const std::string &name)
{
  for (unsigned char idx = 0; idx < QOS_DURABILITY_COUNT; ++idx) {
    STREAM_QOS_DURABILITY temp = static_cast<STREAM_QOS_DURABILITY>(idx);
    if (vpIoTools::toLowerCase(name) == durabilityToString(temp)) {
      return temp;
    }
  }
  return QOS_DURABILITY_UNKNOWN;
}

std::string durabilityList(const std::string &pref, const std::string sep, const std::string &suf)
{
  std::string list = pref;
  for (unsigned char idx = 0; idx < QOS_RELIABILITY_COUNT - 1; ++idx) {
    STREAM_QOS_DURABILITY temp = static_cast<STREAM_QOS_DURABILITY>(idx);
    list += durabilityToString(temp);
    list += sep;
  }
  list += durabilityToString(static_cast<STREAM_QOS_DURABILITY>(QOS_RELIABILITY_COUNT - 1));
  list += suf;
  return list;
}
}
}
