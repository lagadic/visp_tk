#ifndef APRILTAG_TRACKER_HPP
#define APRILTAG_TRACKER_HPP

#include <sensor_msgs/msg/image.hpp>
#include <visp_common/image.hpp>
#include <visp_common/pose.hpp>
#include <visp_tracker_common/BaseTracker.hpp>
#include <visp_tracker_common/msg/april_tag_detection_array.hpp>

#include <visp3/core/vpIoTools.h>
#if defined(VISP_HAVE_DISPLAY) && defined(VISP_HAVE_MODULE_GUI)
#include <visp3/gui/vpDisplayFactory.h>
#endif
#include <visp3/detection/vpDetectorAprilTag.h>

namespace visp_apriltag
{
class AprilTagTracker : public visp_tracker_common::BaseTracker
{
public:
  AprilTagTracker(const std::string &node_name);
  virtual ~AprilTagTracker() = default;

protected:
  /** @name  Initialization */
  ///@{

  /**
   * @copydoc visp_tracker_common::BaseTracker::init_tracker()
   */
  virtual bool init_tracker() override;

  /**
   * @copydoc visp_tracker_common::BaseTracker::init_info_strings()
   */
  virtual void init_info_strings() override;

  ///@}

  /** @name  Callbacks */
  ///@{

  // ----- Subscriptions -----

  /**
   * @brief Color image callback.
   *
   * @param msg The color image.
   */
  void image_callback(const sensor_msgs::msg::Image::ConstSharedPtr &msg);

  // ----- Services -----

  ///@}

  // ----- Services -----

  // ----- Subscribers -----
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr m_rgb_stream_sub; //!< RGB stream subscriber

  // ----- Publisher -----
  rclcpp::Publisher<visp_tracker_common::msg::AprilTagDetectionArray>::SharedPtr m_tags_info_pub; //!< Publisher of the tag IDs, centers and so on.

  // ----- Display-related attributes -----
#if defined(VISP_HAVE_DISPLAY) && defined(VISP_HAVE_MODULE_GUI)
  bool m_display_initialized = false;
  std::shared_ptr<vpDisplay> m_display;
#endif

  // ----- Tracking-related attributes -----
  vpDetectorAprilTag m_tag_detector; //!< The detector.
  std::string m_family_name; //!< Name of the tag family.
  float m_tag_size; //!< Size of the tag, in meters.
  vpImage<unsigned char> m_I; //!< Color image on which the detection is performed.
};
}

#endif
