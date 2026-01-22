#ifndef MBT_TRACKER_HPP
#define MBT_TRACKER_HPP

#include <visp_common/image.hpp>
#include <visp_common/pose.hpp>
#include <visp_common/path_retriever.hpp>

#include <visp3/core/vpConfig.h>
#include <visp3/core/vpImageConvert.h>
#include <visp3/core/vpIoTools.h>
#include <visp3/core/vpTrackingException.h>
#include <visp3/gui/vpDisplayFactory.h>
#include <visp3/mbt/vpMbGenericTracker.h>

#include <visp_tracker_common/BaseMultiModalTracker.hpp>

namespace visp_mbt
{
class MBTTracker : public visp_tracker_common::BaseMultiModalTracker
{
public:
  MBTTracker(const std::string &name);
  virtual ~MBTTracker() = default;
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

  bool init_from_xml();

  bool init_from_json();

  /**
   * @brief Initialize the m_info_strings vector with constant strings to
   * give the user some info.
   */
  virtual void init_info_strings() override;

  /**
   * @brief Check the tracker settings in order to know if the depth
   * is actually required and set m_depth_is_required accordingly.
   */
  virtual void check_requires_depth() override;

  //@}

  /**
   * @copydoc visp_tracker_common::BaseMultiModalTracker::treat_rgb() .
   */
  virtual void treat_rgb(const sensor_msgs::msg::Image::ConstSharedPtr &rgb) override;

  /**
   * @copydoc visp_tracker_common::BaseMultiModalTracker::treat_depth() .
   */
  virtual void treat_depth(const sensor_msgs::msg::Image::ConstSharedPtr &depth) override;

  /**
   * @copydoc visp_tracker_common::BaseMultiModalTracker::track() .
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
  static visp_tracker_common::msg::NamedFeature mbtModelToMsg(const std::vector<std::vector<double>> &model, const std::string &name, const rclcpp::Logger &logger);

  // ----- Services -----

  // ----- Subscribers -----

  // ----- Publisher -----

  // ----- Display-related attributes -----
  vpImage <unsigned char> m_I_depth_display; // Color encoded depth image
  double m_max_z_display; // Maximum depth we want to display
  std::shared_ptr<vpDisplay> m_display; //!< RGB image display
  std::shared_ptr<vpDisplay> m_display_depth; //!< Depth display
  bool m_display_initialized = false; //!< True when the RGB image display is up and running

  // ----- Tracking-related attributes -----
  vpMbGenericTracker m_tracker;
  std::string m_init_file_path;
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
