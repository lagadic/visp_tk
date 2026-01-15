#ifndef BASE_TRACKER_HPP
#define BASE_TRACKER_HPP

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <std_srvs/srv/trigger.hpp>

#include <visp_common/camera.h>
#include <visp_tracker_common/names.hpp>
#include <visp_tracker_common/msg/info_strings.hpp>
#include <visp_tracker_common/msg/named_feature_array.hpp>
#include <visp_tracker_common/msg/named_pose_array.hpp>

#include <visp3/core/vpCameraParameters.h>


namespace visp_tracker_common
{
class BaseTracker : public rclcpp::Node
{
public:
/**
   * @brief Constructor of the abstract base class.
   *
   * @param node_name The name of the node.
   * @param does_publish_features If headless mode is active, set to true if the node must publish 2D points to have a feedback on the remote GUI.
   */
  BaseTracker(const std::string &node_name, const bool &does_publish_features);

  virtual ~BaseTracker() = default;

  /**
   * @brief Initializes the node
   *
   * @return true The initialization went well.
   * @return false Otherwise.
   */
  virtual bool init();

  /**
   * @brief Check if the user asked to quit the node
   *
   * @return true The user wants to quit the node
   * @return false Otherwise
   */
  inline bool has_to_quit() const
  {
    return m_quit;
  }

  /**
   * @brief Method to stop the robot and quit the node.
   *
   */
  virtual void stop_and_quit();

protected:
  /** @name  Initialization */
  //@{

  /**
   * @brief Initilize the tracker used by the servoing node.
   *
   * @return true The initialization went well
   * @return false A problem occured
   */
  virtual bool init_tracker() = 0;

  /**
   * @brief Initialize the m_info_strings vector with constant strings to
   * give the user some info.
   */
  virtual void init_info_strings() = 0;

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
  void color_camera_info_callback(const sensor_msgs::msg::CameraInfo::SharedPtr msg);

  // ----- Services -----

  /**
   * @brief Callback that is called to terminate the node.
   *
   * @param request Trigger signal.
   * @param response Response containing the status and the message.
   */
  void quit_callback(const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
                             std::shared_ptr<std_srvs::srv::Trigger::Response> response);

  /**
   * @brief Callback that is called to stop or start the tracking.
   *
   * @param request Trigger signal.
   * @param response Empty response.
   */
  void switch_tracking_status_callback(const  std::shared_ptr<std_srvs::srv::Trigger::Request> request,
                             std::shared_ptr<std_srvs::srv::Trigger::Response> response);

  /**
   * @brief Callback that is called to stop or start the visual feedback.
   *
   * @param request Trigger signal.
   * @param response Empty response.
   */
  void switch_visual_status_callback(const  std::shared_ptr<std_srvs::srv::Trigger::Request> request,
                             std::shared_ptr<std_srvs::srv::Trigger::Response> response);

  //@}

  // ----- Parameters changes handling -----
  std::shared_ptr<rclcpp::ParameterEventHandler> param_subscriber_;
  std::shared_ptr<rclcpp::ParameterCallbackHandle> cb_handle_;

  // ----- Services -----
  rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr m_quit_srv; //!< Service to quit the node using ros2 service
  rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr m_switch_tracking_status_srv; //!< Service to switch ON/OFF the tracking
  rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr m_switch_visual_srv; //!< Service to switch ON/OFF the visual debbuging

  // ----- Subscribers -----
  rclcpp::Subscription<sensor_msgs::msg::CameraInfo>::SharedPtr m_ref_rgb_cam_info_sub; //!< (Reference, in case of a multi-rgb-camera system) RGB camera parameters subscriber

  // ----- Publisher -----
  rclcpp::Publisher<visp_tracker_common::msg::NamedFeatureArray>::SharedPtr m_features_pub; //!< 2D image points publisher, for remote GUI visualization when headless mode is active
  rclcpp::Publisher<visp_tracker_common::msg::InfoStrings>::SharedPtr m_info_strings_pub; //!< Publisher of the infos that are displayed on screen
  rclcpp::Publisher<visp_tracker_common::msg::NamedPoseArray>::SharedPtr m_poses_pub; //!< Poses publisher, for remote GUI visualization when headless mode is active

  // ----- Display-related attributes -----
  bool m_is_headless_mode = false; //!< True if the GUI is managed by another node
  int m_display_nb_frames_skipped = -1; //!< If positive, the display will be updated only one every m_display_nb_frames_skipped frames, otherwise the display is always updated.
  unsigned int m_frame_cnt = 0; //!< Counter for the display frame skip
  visp_tracker_common::msg::InfoStrings m_info_strings; //!< Vector that contains strings to display on string to give the user some info.
  bool m_visualization_debug = false; //!< Set to true if the node must publish 3D markers to visualize some points of interest in RVIZ

  // ----- Tracking-related attributes -----
  bool m_rgb_cam_info_received = false; //!< Set to true once the color camera parameters have been retrieved.
  std::string m_rgb_camera_topic_name; //!< The name of the (reference, in case of multi-RGB-camera system) color camera topic.
  std::string m_rgb_stream_name; //!< The name of the (reference, in case of multi-RGB-camera system) color image topic.
  vpCameraParameters m_rgb_cam; //!< The (reference, in case of multi-RGB-camera system) color camera parameters

  // ----- Other attributes -----
  std::mutex m_mutex_quit; //!< Mutex to protect m_quit from concurrent access
  bool m_quit = false; //!< Set to true when a SIGINT is received or when the user clicks on the screen
  std::mutex m_mutex_tracking; //!< Mutex to protect m_quit from concurrent access
  bool m_has_to_track = false; //!< Set to true when the tracker must run
  std::mutex m_mutex_visualization; //!< Mutex to protect m_visualization_debug from concurrent access

  static const std::string s_dumb_topic_name; //!< Dumb name to avoid error when a topic name has not been given. The error will be handled in the init() method.
  static const unsigned int s_default_hor_offset; //!< Default horizontal offset to display the m_info_strings information
};
}
#endif
