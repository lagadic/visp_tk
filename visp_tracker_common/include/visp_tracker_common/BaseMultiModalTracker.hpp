#ifndef BASE_MM_TRACKER_HPP
#define BASE_MM_TRACKER_HPP

#include <rclcpp/rclcpp.hpp>

#include <visp_tracker_common/BaseTracker.hpp>
#include <visp_tracker_common/names.hpp>
#include <visp_tracker_common/msg/named_feature.hpp>


namespace visp_tracker_common
{
class BaseMultiModalTracker : public BaseTracker
{
public:
  BaseMultiModalTracker(const std::string &name, const bool &does_publish_features);
  virtual ~BaseMultiModalTracker() = default;

  virtual bool init() override;

protected:
  /** @name  Initialization */
  //@{

  /**
   * @brief Check the tracker settings in order to know if the depth
   * is actually required and set m_depth_is_required accordingly.
   */
  virtual void check_requires_depth() = 0;

  //@}


  /** @name  Callbacks */
  //@{

  // ----- Subscriptions -----

  /**
   * @brief Color camera parameters callback that is called only once to
   * initialize the node internal parameters
   *
   * @param msg Color camera parameters message
   */
  void depth_camera_info_callback(const sensor_msgs::msg::CameraInfo::SharedPtr msg);

  // ----- Services -----

  //@}

  // ----- Parameters changes handling -----

  // ----- Services -----

  // ----- Subscribers -----
  rclcpp::Subscription<sensor_msgs::msg::CameraInfo>::SharedPtr m_depth_cam_info_sub; //!< Depth camera parameters subscriber

  // ----- Publisher -----

  // ----- Display-related attributes -----

  // ----- Tracking-related attributes -----
  bool m_depth_cam_info_received = false; //!< Set to true once the color camera parameters have been retrieved.
  std::string m_depth_camera_topic_name; //!< The name of the depth camera topic.
  std::string m_depth_stream_name; //!< The name of the depth image topic.
  vpCameraParameters m_depth_cam; //!< The depth camera parameters.
  bool m_depth_is_required = false; //!< If true, it means that the tracker requires a depth stream to run.

  // ----- Other attributes -----

};
}
#endif
