#ifndef APRILTAG_TRACKER_HPP
#define APRILTAG_TRACKER_HPP

#include <sensor_msgs/msg/image.hpp>
#include <visp_tracker_common/BaseTracker.hpp>

#include <visp3/core/vpIoTools.h>
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
  //@{

  /**
   * @brief Initilize the tracker used by the servoing node.
   *
   * @return true The initialization went well
   * @return false A problem occured
   */
  virtual bool init_tracker() override;

  /**
   * @brief Initialize the m_info_strings vector with constant strings to
   * give the user some info.
   */
  virtual void init_info_strings() override;

  //@}

  /** @name  Callbacks */
  //@{

  // ----- Subscriptions -----

  /**
   * @brief Color image callback.
   *
   * @param msg The color image.
   */
  void image_callback(const sensor_msgs::msg::Image::ConstSharedPtr &msg);

  // ----- Services -----

  //@}

  /** @name  Enum and associated tools */
  /**
   * @brief Cast a \b vpDetectorAprilTag::vpAprilTagFamily enum value into a \b std::stirng.
   *
   * @param family The type of 2D features we want to cast into a string.
   * @return std::string The name of the \b vpDetectorAprilTag::vpAprilTagFamily enum value.
   */
  static std::string tagFamilyToString(const vpDetectorAprilTag::vpAprilTagFamily &family);

  /**
   * @brief Cast a string into a \b vpDetectorAprilTag::vpAprilTagFamily enum value.
   * If \b name is not found, throw an error .
   *
   * @param name The name of the display mode.
   * @return DisplayMode The corresponding \b TagType enum value, or throw an error if not found.
   */
  static vpDetectorAprilTag::vpAprilTagFamily tagFamilyFromString(const std::string &name);

  /**
   * @brief Create a string that lists the different \b vpDetectorAprilTag::vpAprilTagFamily available.
   *
   * @param prefix The string that must prefix the list of modes.
   * @param sep The separator between the different modes.
   * @param suffix The string that must suffix the list of modes.
   * @return std::string The list containing the different modes.
   */
  static std::string getAvailableTagFamily(const std::string &prefix = "< ", const std::string &sep = " , ", const std::string &suffix = " >");

  /**
   * @brief Cast a \b vpDetectorAprilTag::vpPoseEstimationMethod enum value into a \b std::stirng.
   *
   * @param method The type of 2D features we want to cast into a string.
   * @return std::string The name of the \b vpDetectorAprilTag::vpPoseEstimationMethod enum value.
   */
  static std::string poseMethodToString(const vpDetectorAprilTag::vpPoseEstimationMethod &method);

  /**
   * @brief Cast a string into a \b vpDetectorAprilTag::vpPoseEstimationMethod enum value.
   * If \b name is not found, throw an error .
   *
   * @param name The name of the display mode.
   * @return DisplayMode The corresponding \b TagType enum value, or throw an error if not found.
   */
  static vpDetectorAprilTag::vpPoseEstimationMethod poseMethodFromString(const std::string &name);

  /**
   * @brief Create a string that lists the different \b vpDetectorAprilTag::vpPoseEstimationMethod available.
   *
   * @param prefix The string that must prefix the list of modes.
   * @param sep The separator between the different modes.
   * @param suffix The string that must suffix the list of modes.
   * @return std::string The list containing the different modes.
   */
  static std::string getAvailablePoseMethod(const std::string &prefix = "< ", const std::string &sep = " , ", const std::string &suffix = " >");
  //@}

  // ----- Services -----

  // ----- Subscribers -----
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr m_rgb_stream_sub; //!< RGB stream subscriber

  // ----- Publisher -----

  // ----- Display-related attributes -----

  // ----- Tracking-related attributes -----
};
}

#endif
