#ifndef RBT_TRACKER_HPP
#define RBT_TRACKER_HPP

#include <visp_common/image.hpp>
#include <visp_common/pose.hpp>
#include <visp_common/path_retriever.hpp>

#include <visp3/core/vpConfig.h>
#include <visp3/ar/vpPanda3DFrameworkManager.h>
#include <visp3/core/vpImageConvert.h>
#include <visp3/core/vpIoTools.h>
#if defined(VISP_HAVE_DISPLAY) && defined(VISP_HAVE_MODULE_GUI)
#include <visp3/gui/vpDisplayFactory.h>
#endif
#include <visp3/rbt/vpRBTracker.h>
#include <visp3/rbt/vpRBDriftDetector.h>

#include <visp_tracker_common/BaseMultiModalTracker.hpp>

#ifdef VISP_HAVE_NLOHMANN_JSON
#include VISP_NLOHMANN_JSON(json.hpp)
#else
#error "ViSP must have been compiled with nlohmann-json library to use this package"
#endif

#ifdef VISP_HAVE_OPENMP
#include <omp.h>
#endif

#ifdef ENABLE_VISP_NAMESPACE
using namespace VISP_NAMESPACE_NAME;
#endif

namespace visp_rbt
{
class RBTTracker : public visp_tracker_common::BaseMultiModalTracker
{
public:
  RBTTracker(const std::string &name);

  virtual ~RBTTracker()
  {
    RCLCPP_INFO(this->get_logger(), "Destructor called, closing Panda3D instance");
    vpPanda3DFrameworkManager::getInstance().exit();
    RCLCPP_INFO(this->get_logger(), "Panda instance has been closed");
  }

  /**
   * @copydoc visp_tracker_common::BaseTracker::init()
   */
  // virtual bool init() override;
protected:
  /** @name  Initialization */
  ///@{
  /**
   * @copydoc visp_tracker_common::BaseTracker::init_tracker()
   */
  virtual bool init_tracker() override;

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
   * @brief Initialize the tracker.
   *
   * @param[out] cMo The pose after initialization.
   * @return true Initialization went well.
   * @return false The tracker is still uninitialized.
   */
  virtual bool init_tracking(vpHomogeneousMatrix &cMo, bool &display_frame);

  /**
   * @brief Perform the tracking.
   *
   * @param[out] cMo The pose resulting from the tracking.
   * @param[out] vec_info Vector that contains info to display on screen
   * @return true Tracking was successful.
   * @return false Otherwise.
   */
  virtual bool perform_tracking(vpHomogeneousMatrix &cMo, std::vector<std::string> &vec_info);

  // ----- Services -----

  // ----- Subscribers -----

  // ----- Publisher -----

  // ----- Display-related attributes -----
#if defined(VISP_HAVE_DISPLAY) && defined(VISP_HAVE_MODULE_GUI)
  vpImage <unsigned char> m_I_depth_display; // Color encoded depth image
  double m_max_z_display; // Maximum depth we want to display
  std::shared_ptr<vpDisplay> m_display; //!< RGB image display
  std::shared_ptr<vpDisplay> m_display_uchar; //!< Gray-scale image display
  std::shared_ptr<vpDisplay> m_display_depth; //!< Depth display
  bool m_display_initialized = false; //!< True when the RGB image display is up and running
#endif

  // ----- Tracking-related attributes -----
  bool m_tracker_initialized = false; //!< True when the tracker is correctly initialized, false when the tracking was lost or never began.
  bool m_tracker_cams_set = false; //!< True once the camera parameters of the tracker will be set.
  std::string m_init_file_path; //!< Path towards the init file that contains the 3D coordinates of the points to click to initialize the tracker.
  vpImage<unsigned char> m_I; //!< Gray-scale image
  vpImage<vpRGBa> m_Ic; //!< Color image
  vpImage <float> m_I_depth; //!< Depth image
  vpRBTracker m_tracker;
};
}

#endif
