#ifndef MBT_TRACKER_HPP
#define MBT_TRACKER_HPP

#include <visp_common/image.hpp>
#include <visp_common/pose.hpp>
#include <visp_common/path_retriever.hpp>

#include <visp3/core/vpConfig.h>
#include <visp3/core/vpImageConvert.h>
#include <visp3/core/vpIoTools.h>
#include <visp3/core/vpTrackingException.h>
#if defined(VISP_HAVE_DISPLAY) && defined(VISP_HAVE_MODULE_GUI)
#include <visp3/gui/vpDisplayFactory.h>
#endif
#include <visp3/mbt/vpMbGenericTracker.h>

#include <visp_tracker_common/BaseMultiModalTracker.hpp>

#ifdef VISP_HAVE_NLOHMANN_JSON
#include VISP_NLOHMANN_JSON(json.hpp)
#endif

namespace visp_mbt
{
class MBTTracker : public visp_tracker_common::BaseMultiModalTracker
{
public:
  MBTTracker(const std::string &name);
  virtual ~MBTTracker() = default;

  /**
   * @copydoc visp_tracker_common::BaseTracker::init()
   */
  virtual bool init() override;
protected:
  /** @name  Initialization */
  ///@{
  /**
   * @copydoc visp_tracker_common::BaseTracker::init_tracker()
   */
  virtual bool init_tracker() override;

  /**
   * @brief Initilize the tracker using an XML file and some node parameters.
   *
   * @param config_file_path Path towards the configuration file.
   * @return true The initialization went well.
   * @return false An error occured.
   */
  bool init_from_xml(const std::string &config_file_path);

  /**
   * @brief Initilize the tracker using a JSON file and some node parameters.
   *
   * @param config_file_path Path towards the configuration file.
   * @return true The initialization went well.
   * @return false An error occured.
   */
  bool init_from_json(const std::string &config_file_path);

  /**
   * @copydoc visp_tracker_common::BaseTracker::init_info_strings()
   */
  virtual void init_info_strings() override;

  /**
   * @copydoc visp_tracker_common::BaseMultiModalTracker::check_requires_depth()
   */
  virtual void check_requires_depth() override;
  ///@}

  /**
   * @copydoc visp_tracker_common::BaseMultiModalTracker::treat_rgb()
   */
  virtual void treat_rgb(const sensor_msgs::msg::Image::ConstSharedPtr &rgb) override;

  /**
   * @copydoc visp_tracker_common::BaseMultiModalTracker::treat_depth()
   */
  virtual void treat_depth(const sensor_msgs::msg::Image::ConstSharedPtr &depth) override;

  /**
   * @copydoc visp_tracker_common::BaseMultiModalTracker::track()
   */
  virtual void track() override;

  /**
   * @brief Cast a vpMbGenericTracker model into a visp_tracker_common::msg::NamedFeature message.
   *
   * @param model Model of the tracker.
   * @param name Name to attach to the model.
   * @param logger rclcpp::Logger to display messages if needed.
   * @return visp_tracker_common::msg::NamedFeature Message containing the model.
   */
  static visp_tracker_common::msg::NamedFeature mbt_model_to_msg(const std::vector<std::vector<double>> &model, const std::string &name, const rclcpp::Logger &logger);

  /**
   * @brief Check if the model file parameter is correctly set, perform path substitution if needed
   * and store the result in value.
   *
   * @param param The ROS2 parameter from which the value must be read.
   * @param logger rclcpp::Logger to display messages if needed.
   * @param required If true, the parameter cannot be empty.
   * @param value The resulting path after substitution (if needed).
   */
  static bool check_model_parameter(const rclcpp::Parameter &param, const rclcpp::Logger &logger, const bool &required, std::string &value);

  /**
   * @brief Cast a vpMbGenericTracker::vpTrackerType into its corresponding name.
   *
   * @param type The vpMbGenericTracker::vpTrackerType we want to know the name.
   * @return std::string The corresponding name.
   */
  static std::string trackerTypeToStr(const vpMbGenericTracker::vpTrackerType &type);

  /**
   * @brief Cast a name into its corresponding vpMbGenericTracker::vpTrackerType.
   *
   * @param name The name of the tracker type.
   * @return vpMbGenericTracker::vpTrackerType The corresponding vpMbGenericTracker::vpTrackerType.
   */
  static vpMbGenericTracker::vpTrackerType trackerTypeFromStr(const std::string &name);

  /**
   * @brief Get the list of names of the available vpMbGenericTracker::vpTrackerType.
   *
   * @param prefix The prefix of the list.
   * @param sep The separator for the list.
   * @param suffix The suffix for the list.
   * @return std::string The resulting list of names.
   */
  static std::string getAvailableTrackerType(const std::string &prefix = "< ", const std::string &sep = " , ", const std::string &suffix = " >");

  // ----- Services -----

  // ----- Subscribers -----

  // ----- Publisher -----

  // ----- Display-related attributes -----
#if defined(VISP_HAVE_DISPLAY) && defined(VISP_HAVE_MODULE_GUI)
  vpImage <unsigned char> m_I_depth_display; // Color encoded depth image
  double m_max_z_display; // Maximum depth we want to display
  std::shared_ptr<vpDisplay> m_display; //!< RGB image display
  std::shared_ptr<vpDisplay> m_display_depth; //!< Depth display
  bool m_display_initialized = false; //!< True when the RGB image display is up and running
#endif

  // ----- Tracking-related attributes -----
  std::shared_ptr<vpMbGenericTracker> m_tracker;
  std::string m_init_file_path; //!< Path towards the init file that contains the 3D coordinates of the points to click to initialize the tracker.
  bool m_load_models_from_params = false; //!< If true, the model files must be read from the node parameters.
  std::string m_rgb_model; //!< If the models must be read from the node parameters, the path towards the model of the RGB tracker.
  std::string m_depth_model; //!< If the models must be read from the node parameters, the path towards the model of the depth tracker.
  std::vector<std::string> m_color_trackers_name; //!< Name(s) of the tracker(s) based on color information.
  std::vector<std::string> m_depth_trackers_name; //!< Name(s) of the tracker(s) based on depth information.
  bool m_tracker_initialized = false; //!< True when the tracker is correctly initialized, false when the tracking was lost or never began.
  bool m_tracker_cams_set = false; //!< True once the camera parameters of the tracker will be set.
  vpImage<unsigned char> m_I; //!< Gray-scale image.
  vpImage<vpRGBa> m_Ic; //!< RGB image.
  std::vector<vpColVector> m_pointcloud; //!< Depth information for the vpMbGenericTracker if it requires depth information.
  std::map<std::string, const vpImage<vpRGBa> *> m_map_img; //!< Map that contains the name(s) of the color tracker(s) and a pointer towards the current color image.
  std::map<std::string, const std::vector<vpColVector> *> m_map_pc; //!< Map that contains the name(s) of the depth tracker(s) and a pointer towards the current depth information.
  std::map<std::string, unsigned int> m_map_pcw; //!< Map that contains the width of the depth information.
  std::map<std::string, unsigned int> m_map_pch; //!< Map that contains the height of the depth information.
};
}

#endif
