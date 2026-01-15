#include <visp_mbt/MBTTracker.hpp>

namespace visp_mbt
{
MBTTracker::MBTTracker(const std::string &name) : visp_tracker_common::BaseMultiModalTracker(name, true)
{

}

bool MBTTracker::init_tracker()
{
  return true;
}

void MBTTracker::init_info_strings() { }

void MBTTracker::check_requires_depth() { }

}
