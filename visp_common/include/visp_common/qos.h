#ifndef visp_common_QOS_H
#define visp_common_QOS_H

#include <string>
#include <visp3/core/vpIoTools.h>

namespace visp_common
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

#endif
