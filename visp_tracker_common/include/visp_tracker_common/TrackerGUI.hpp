#ifndef TRACKER_GUI_HPP
#define TRACKER_GUI_HPP

// ---- Base ROS ----
#include <rclcpp/rclcpp.hpp>
// ---- ROS Images ----
#include <image_transport/image_transport.hpp>
// ---- ROS Messages ----
#include <sensor_msgs/msg/image.hpp>
// ---- ROS Services ----
#include <std_srvs/srv/trigger.hpp>
// ---- ViSP ROS packages ----
#include <visp_common/pose.hpp>
#include <visp_common/camera.hpp>
#include <visp_common/image.hpp>
#include <visp_common/qos.hpp>
#include <visp_tracker_common/names.hpp>
#include <visp_tracker_common/msg/info_strings.hpp>
#include <visp_tracker_common/msg/named_feature_array.hpp>
#include <visp_tracker_common/msg/named_pose_array.hpp>
// ---- ViSP includes ----
#include <visp3/core/vpConfig.h>
#include <visp3/core/vpCameraParameters.h>
#include <visp3/core/vpHomogeneousMatrix.h>
#include <visp3/core/vpImageConvert.h>
#include <visp3/core/vpImage.h>
#include <visp3/core/vpIoTools.h>
#include <visp3/core/vpQuaternionVector.h>
#include <visp3/core/vpRotationMatrix.h>
#include <visp3/core/vpTime.h>
#include <visp3/core/vpTranslationVector.h>
#include <visp3/core/vpVelocityTwistMatrix.h>
#include <visp3/gui/vpDisplayFactory.h>

// ---- System includes ----
#include <memory>
#include <mutex>
#include <optional>
#ifdef VISP_HAVE_OPENMP
#include <omp.h>
#endif

namespace visp_tracker_common
{
class TrackerGUI : public rclcpp::Node
{
public:
  TrackerGUI(const std::string &node_name);

  virtual ~TrackerGUI() = default;

  /**
   * @brief Self is required to initialize the m_it attribute.
   *
   * @param self A std::shared_ptr towards the current object.
   * @return true Initialization went well.
   * @return false A problem occured.
   */
  bool init(std::shared_ptr<rclcpp::Node> self);

  /**
   * @brief Indicates if the node must still run or should be stopped.
   *
   * @return true The node should continue to run.
   * @return false The node should stop.
   */
  inline bool has_to_run() const { return m_run; }

  /**
   * @brief Method to properly stop the node.
   */
  void quit();

protected:
  /** @name  Callbacks */
  //@{

  /**
   * @brief Camera parameters callback that initialize the RGB camera parameters
   *
   * @param msg Camera parameters message
   */
  void camera_info_callback(const sensor_msgs::msg::CameraInfo::SharedPtr msg);

  /**
   * @brief Color image callback, that manages the display of both images
   *
   * @param msg The color image
   */
  void image_callback(const sensor_msgs::msg::Image::ConstSharedPtr &msg);

  /**
   * @brief Callback for the depth image, if used.
   *
   * @param msg
   */
  void depth_callback(const sensor_msgs::msg::Image::ConstSharedPtr &msg);

  /**
   * @brief Callback for named 2D features.
   *
   * @param msg
   */
  void features_callback(const visp_tracker_common::msg::NamedFeatureArray::ConstSharedPtr msg);

  /**
   * @brief Callback for the informational strings to display.
   *
   * @param msg
   */
  void info_callback(const visp_tracker_common::msg::InfoStrings::ConstSharedPtr msg);

  /**
   * @brief Callback for named 2D poses.
   *
   * @param msg
   */
  void poses_callback(const visp_tracker_common::msg::NamedPoseArray::ConstSharedPtr msg);
  //@}

  /** @name  Enum and associated tools */
  //@{
  /**
   * @brief Enumeration permitting to choose the type of object to represent the 2D feature points
   * published by the client node.
   */
  typedef enum FeaturesType
  {
    POINT = 0, /*!< Use points to represent the 2D features published by the client node.*/
    CROSS = 1, /*!< Use crosses to represent the 2D features published by the client node.*/
    TYPE_COUNT = 2
  } FeaturesType;

  /**
   * @brief Cast a \b FeaturesType enum value into a \b std::stirng.
   *
   * @param mode The type of 2D features we want to cast into a string.
   * @return std::string The name of the \b FeaturesType enum value.
   */
  static std::string featuresTypeToString(const FeaturesType &mode);

  /**
   * @brief Cast a string into a \b FeaturesType enum value.
   * If \b name is not found, return \b FeaturesType::TYPE_COUNT .
   *
   * @param name The name of the display mode.
   * @return DisplayMode The corresponding \b FeaturesType enum value, or \b FeaturesType::TYPE_COUNT if not found.
   */
  static FeaturesType featuresTypeFromString(const std::string &name);

  /**
   * @brief Create a string that lists the different \b FeaturesType available.
   *
   * @param prefix The string that must prefix the list of modes.
   * @param sep The separator between the different modes.
   * @param suffix The string that must suffix the list of modes.
   * @return std::string The list containing the different modes.
   */
  static std::string getAvailableFeaturesType(const std::string &prefix = "< ", const std::string &sep = " , ", const std::string &suffix = " >");
  //@}

  // ----- Services -----
  rclcpp::Node::SharedPtr m_service_node; //!< Node to wait until a service is completed.
  std_srvs::srv::Trigger::Request::SharedPtr m_quit_request; //!< Request for the quit service.
  std::shared_ptr<rclcpp::Client<std_srvs::srv::Trigger>> m_client_quit; //!< Client to the quit service.
  std_srvs::srv::Trigger::Request::SharedPtr m_switch_request; //!< Request for any switch service.
  std::shared_ptr<rclcpp::Client<std_srvs::srv::Trigger>> m_client_switch_tracking; //!< Client to turn ON/OFF the tracking.
  std::shared_ptr<rclcpp::Client<std_srvs::srv::Trigger>> m_client_switch_visualization; //!< Client to turn ON/OFF the visualization of 2D features.

  // ----- Subscribers -----
  rclcpp::Node::SharedPtr m_it_node;
  std::shared_ptr<image_transport::ImageTransport> m_it;
  std::shared_ptr<image_transport::TransportHints> m_hints;
  image_transport::Subscriber m_sub_color;
  // image_transport::ImageTransport m_it_depth;
  std::shared_ptr<image_transport::TransportHints> m_hints_depth;
  image_transport::Subscriber m_sub_depth;
  rclcpp::Subscription<sensor_msgs::msg::CameraInfo>::SharedPtr m_rgb_cam_sub; //!< Subscriber to the RGB camera topic.
  rclcpp::Subscription<visp_tracker_common::msg::NamedFeatureArray>::SharedPtr m_feat_2D_sub; //!< Subscriber to the 2D features topic.
  rclcpp::Subscription<visp_tracker_common::msg::InfoStrings>::SharedPtr m_info_strings_sub; //!< Subscriber to the info strings topic.
  rclcpp::Subscription<visp_tracker_common::msg::NamedPoseArray>::SharedPtr m_poses_sub; //!< Subscriber to the poses topic.
  visp_tracker_common::msg::NamedFeatureArray m_feature_array; //!< The 2D features published by the tracker that must be displayed.
  std::mutex m_mutex_features; //!< Mutex that protects the 2D features array.
  visp_tracker_common::msg::NamedPoseArray m_pose_array; //!< The named poses published by the tracker.
  std::mutex m_mutex_poses; //!< Mutex that protects the poses.
  visp_tracker_common::msg::InfoStrings m_vec_info; //!< Strings to display on screen to give info to the user.
  std::mutex m_mutex_info; //!< Mutex that protects the info.

  // ----- Display-related -----
  bool m_use_depth = false; //!< If true, the depth image will be displayed using a color encoding.
  std::shared_ptr<vpDisplay> m_display_color; //!< Display for the RGB image.
  std::shared_ptr<vpDisplay> m_display_depth; //!< Display for the depth image, if m_use_depth is true.

  vpImage<vpRGBa> m_I; //!< RGB image.
  std::mutex m_mutex_Id; //!< Mutex that protects depth image.
  std::optional<vpImage<vpRGBa>> m_opt_Id = std::nullopt; //!< Color-encoded depth image for rendering purpous, if m_use_depth is true.
  std::optional<uint16_t> m_opt_min_depth = std::nullopt; //!< Min depth to render.
  std::optional<uint16_t> m_opt_max_depth = std::nullopt; //!< Max depth to render.
  int m_display_nb_frames_skipped = -1; //!< -1 to display all the frames, either will display 1 frame every N frames.
  int m_frame_cnt = 0; //!< Counter if not all the frames must be displayed.
  unsigned int m_features_thickness = 1; //!< Thickness of the 2D features.
  FeaturesType m_features_type = FeaturesType::POINT; //!< How to render 2D points on screen.
  std::optional<vpCameraParameters> m_opt_rgb_cam = std::nullopt; //!< To display the poses.

  // ----- Others -----
  bool m_run = true; //!< When set to false, the TrackerGUI will shutdown.
  std::mutex m_mutex_run; //!< Mutex that protects m_run.
  std::string m_client_node_name; //!< Name of the tracker node, that prepends the different topics.
};
}

#endif
