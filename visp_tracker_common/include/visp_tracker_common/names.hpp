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
extern const std::string poses_topic_name; //!< Name of the topic on which are published some poses of interest.
}
#endif
