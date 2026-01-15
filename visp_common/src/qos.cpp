#include <visp_common/qos.h>

namespace visp_common
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
