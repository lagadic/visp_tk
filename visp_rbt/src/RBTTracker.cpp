#include <visp_rbt/RBTTracker.hpp>

namespace visp_rbt
{
RBTTracker::RBTTracker(const std::string &name) : visp_tracker_common::BaseMultiModalTracker(name, true)
{
  //////////////////////////////////////////////////////////////////////
  //                        ROS2 PARAMETERS                           //
  //////////////////////////////////////////////////////////////////////

  // // ---- Parameters related to the services ----

  // // ---- Parameters related to the publishers / subscribers ----

  // // ---- Others ----
  auto init_file_param = rclcpp::Parameter();
  auto init_param_desc = rcl_interfaces::msg::ParameterDescriptor {};
  init_param_desc.description = "This parameter must be set to the path towards the file that contains the init points.";
  this->declare_parameter("init_file", "", init_param_desc);
  this->get_parameter("init_file", init_file_param);
  m_init_file_path = init_file_param.as_string();

#if defined(VISP_HAVE_DISPLAY) && defined(VISP_HAVE_MODULE_GUI)
  auto max_z_param = rclcpp::Parameter();
  auto max_z_param_desc = rcl_interfaces::msg::ParameterDescriptor {};
  max_z_param_desc.description = "This parameter permits the maximum depth we want to display.";
  this->declare_parameter("max_z_display", 2.0, max_z_param_desc);
  this->get_parameter("max_z_display", max_z_param);
  m_max_z_display = max_z_param.as_double();
#endif

  //////////////////////////////////////////////////////////////////////
  //                        ROS2 SERVICES                             //
  //////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////
  //                        ROS2 PUB/SUB                              //
  //////////////////////////////////////////////////////////////////////
}

//////////////////////////////////////////////////////////////////////
//                        INITIALIZATION                            //
//////////////////////////////////////////////////////////////////////

bool RBTTracker::init_tracker()
{
  // Retrieving init file
  if (m_init_method == BaseTracker::CLICK) {
    if (m_init_file_path.empty()) {
      RCLCPP_ERROR(this->get_logger(), "init file path is empty");
      return false;
    }
    std::vector<std::string> split = vpIoTools::splitChain(m_init_file_path, "://");
    m_init_file_path = visp_common::path::path_retriever(m_init_file_path);
    if (m_init_file_path.empty() && split.size() > 1) {
      // This can occur if the package we tried to load from is not sourced
      RCLCPP_ERROR(this->get_logger(), "Failed to locate package %s to load the init file %s", split[0].c_str(), split[1].c_str());
      return false;
    }
    else if (!vpIoTools::checkFilename(m_init_file_path)) {
      RCLCPP_ERROR(this->get_logger(), "init file %s does not exist", m_init_file_path.c_str());
      return false;
    }
    RCLCPP_INFO(this->get_logger(), "RBT will use the init file %s", m_init_file_path.c_str());
  }

  // Retrieving config file
  std::string config_file_path = this->get_parameter("config_file").as_string();
  if (config_file_path.empty()) {
    RCLCPP_ERROR(this->get_logger(), "config file path is empty");
    return false;
  }

  std::vector<std::string> split = vpIoTools::splitChain(config_file_path, "://");
  config_file_path = visp_common::path::path_retriever(config_file_path);
  if (config_file_path.empty() && split.size() > 1) {
    // This can occur if the package we tried to load from is not sourced
    RCLCPP_ERROR(this->get_logger(), "Failed to locate package %s to load the config file %s", split[0].c_str(), split[1].c_str());
    return false;
  }
  else if (!vpIoTools::checkFilename(config_file_path)) {
    RCLCPP_ERROR(this->get_logger(), "config file %s does not exist", config_file_path.c_str());
    return false;
  }
  RCLCPP_INFO(this->get_logger(), "RBT will use the config file %s", config_file_path.c_str());

  if (config_file_path.find(".json") != std::string::npos) {
    return init_from_json(config_file_path);
  }
  else {
    RCLCPP_ERROR(this->get_logger(), "config file %s is not a JSON file", config_file_path.c_str());
    return false;
  }
}

bool RBTTracker::init_from_json(const std::string &config_file_path)
{
  using json = nlohmann::json;

  std::ifstream file(config_file_path);
  if (!file.good()) {
    RCLCPP_ERROR_STREAM(this->get_logger(), "Problem opening file " << config_file_path << ". Make sure it exists and is readable.");
    return false;
  }
  json global_settings;
  try {
    global_settings = json::parse(file);
  }
  catch (json::parse_error &e) {
    RCLCPP_ERROR_STREAM(this->get_logger(), "Could not parse JSON file '" << config_file_path << "' .");
    RCLCPP_ERROR_STREAM(this->get_logger(), "Error: " << e.what());
    RCLCPP_ERROR_STREAM(this->get_logger(), "Byte position of error: " << e.byte);
    return false;
  }

  // Parsing potential package:// used in the model sections
  if (!global_settings.contains("model")) {
    RCLCPP_ERROR(this->get_logger(), " Configuration file %s does not contain the 'model' key with associated value.", config_file_path.c_str());
    return false;
  }

  std::string model = global_settings["model"].get<std::string>();
  if (model.empty()) {
    RCLCPP_ERROR(this->get_logger(), " Configuration file %s contain the 'model' key but its associated value is empty.", config_file_path.c_str());
    return false;
  }

  model = visp_common::path::path_retriever(model);
  if (model.empty()) {
    RCLCPP_ERROR(this->get_logger(), " Configuration file %s contain the 'model' key but its associated value %s is uncorrect.", config_file_path.c_str(), global_settings["model"].get<std::string>().c_str());
    std::vector<std::string> split = vpIoTools::splitChain(global_settings["model"].get<std::string>(), "://");
    if (split.size()> 1) {
      RCLCPP_ERROR(this->get_logger(), " The package %s is not known, have you sourced it ?.", split[0].c_str());
    }
    return false;
  }

  if (!vpIoTools::checkFilename(model)) {
    RCLCPP_ERROR(this->get_logger(), "Could not find the model file %s", model.c_str());
    return false;
  }

  // read config file
  m_tracker.loadConfigurationFile(config_file_path);
  m_tracker.setModelPath(model);

  return true;
}

void RBTTracker::init_info_strings()
{
  m_info_nb_static = m_info_strings.info_strings.size();
}

void RBTTracker::check_requires_depth()
{
  auto trackers = m_tracker.getFeatureTrackers();
  for (const auto &trackerptr : trackers) {
    if (trackerptr) {
      if (trackerptr->requiresDepth()) {
        m_depth_is_required = true;
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////
//                        OTHERS                                    //
//////////////////////////////////////////////////////////////////////
void RBTTracker::treat_rgb(const sensor_msgs::msg::Image::ConstSharedPtr &rgb)
{
  if ((!m_rgb_cam_info_received) || (m_depth_is_required && !m_depth_cam_info_received)) {
    return;
  }

  m_Ic = std::move(visp_common::image::toVispImageRGBa(*rgb));

  vpImageConvert::convert(m_Ic, m_I);
}

void RBTTracker::treat_depth(const sensor_msgs::msg::Image::ConstSharedPtr &depth)
{
  if (!m_depth_cam_info_received) {
    return;
  }

  // Convert ROS2 image to ViSP image
  const vpImage<uint16_t> &I_depth_raw = visp_common::image::toVispImageUint16(*depth);

  static bool once = true;
  if (once) {
    m_I_depth.init(I_depth_raw.getHeight(), I_depth_raw.getWidth());
    if (!m_is_headless_mode) {
      // Initialize depth display
#if defined(VISP_HAVE_DISPLAY) && defined(VISP_HAVE_MODULE_GUI)
      m_I_depth_display.resize(m_I_depth.getHeight(), m_I_depth.getWidth());
      m_display_depth = vpDisplayFactory::createDisplay(m_I_depth_display);
#endif
    }
    once = false;
  }

  // Clear the point cloud
  int width = I_depth_raw.getWidth();
  int size = I_depth_raw.getSize();
  int idxstart = 0, idxstop = size;
  int j(0), i(0);
#ifdef VISP_HAVE_OPENMP
  int iam, nt, ipoints, npoints(size);
#pragma omp parallel default(shared) private(iam, nt, ipoints, idxstart, idxstop, j, i)
  {
    iam = omp_get_thread_num();
    nt = omp_get_num_threads();
    ipoints = npoints / nt;
    // size of partition
    idxstart = iam * ipoints; // starting array index
    if (iam == nt-1) {
      // last thread may do more
      ipoints = npoints - idxstart;
    }
    idxstop = idxstart + ipoints;
    j = idxstart % width;
    i = idxstart / width;
#endif
    for (int idx = idxstart; idx < idxstop; ++idx) {
      uint16_t Z_raw = I_depth_raw.bitmap[idx];
      double Z = Z_raw * m_z_factor;
      if ((Z > 0.) && (!std::isnan(Z_raw)) && std::isfinite(Z)) {
        m_I_depth.bitmap[idx] = Z;
#if defined(VISP_HAVE_DISPLAY) && defined(VISP_HAVE_MODULE_GUI)
        if (!m_is_headless_mode) {
          m_I_depth_display.bitmap[idx] = Z > m_max_z_display ? 0 : static_cast<unsigned int>((Z / m_max_z_display) * 255.f);
        }
#endif
      }
#if defined(VISP_HAVE_DISPLAY) && defined(VISP_HAVE_MODULE_GUI)
      else if (!m_is_headless_mode) {
        m_I_depth_display.bitmap[idx] = 0;
      }
#endif
      // Updating column index
      ++j;
      if (j == width) {
        // Reached the end of a column, updating row index and resetting column index
        j = 0;
        ++i;
      }
    }
#ifdef VISP_HAVE_OPENMP
  }
#endif
}

void RBTTracker::track()
{
  bool quit = false;
  {
    std::scoped_lock lock(m_mutex_quit);
    quit = m_quit;
  }

  if (quit || (!m_rgb_cam_info_received) || ((!m_depth_cam_info_received) && m_depth_is_required)) {
    RCLCPP_DEBUG(this->get_logger(), "Waiting for the camera parameters to be ready...");
    return;
  }

  if (!m_tracker_cams_set) {
    m_tracker.setCameraParameters(m_rgb_cam, m_I.getHeight(), m_I.getWidth());
    m_tracker.startTracking();
    m_tracker_cams_set = true;
  }

  // Check if frame has to be displayed
#if defined(VISP_HAVE_DISPLAY) && defined(VISP_HAVE_MODULE_GUI)
  bool display_frame = ((!m_is_headless_mode) || ((!m_tracker_initialized) && m_has_to_track && (m_init_method == BaseTracker::CLICK))) &&((m_display_nb_frames_skipped <= 0) || ((m_frame_cnt % m_display_nb_frames_skipped) == 0));

  if (display_frame) {

    if (!m_display_initialized) {
      m_display_uchar = vpDisplayFactory::createDisplay(m_I);
      m_display = vpDisplayFactory::createDisplay(m_Ic);
      if (!m_is_headless_mode) {
        // The depth display is not used for the initialization, only for display when not using the remote GUI
        m_display_depth = vpDisplayFactory::createDisplay(m_I_depth_display);
      }
      m_display_initialized = true;
    }

    vpDisplay::display(m_I);
    vpDisplay::display(m_Ic);
    vpDisplay::display(m_I_depth_display);
    {
      std::stringstream ss;
      ss << "Left click to " << (m_has_to_track ? "stop" : "start") << " the tracking, right click to quit.";
      vpDisplay::displayText(m_Ic, 20, 20, ss.str(), vpColor::red);
    }
  }
#endif

  vpHomogeneousMatrix cMo;
  if (m_has_to_track) {
    RCLCPP_DEBUG(this->get_logger(), "Starting tracking");
    if (!m_tracker_initialized) {
      if (m_init_method == BaseTracker::CLICK) {
#if defined(VISP_HAVE_DISPLAY) && defined(VISP_HAVE_MODULE_GUI)
        RCLCPP_DEBUG(this->get_logger(), "Initializing tracker by click...");
        m_tracker.initClick(m_Ic, m_init_file_path, true);
        m_tracker.getPose(cMo);
        m_tracker_initialized = true;
        if (m_is_headless_mode) {
          vpDisplay::close(m_I);
          vpDisplay::close(m_Ic);
          m_display_uchar.reset();
          m_display.reset();
          // Depth display is not created in headless mode, no need to close it nor to reset it
          m_display_initialized = false;
          display_frame = false;
        }
#else
        throw(vpException(vpException::fatalError, "The tracker cannot be initialized by click if the module visp_gui is not available and/or if a GUI library is not installed. See https://visp-doc.inria.fr/doxygen/visp-daily/supported-third-parties.html"));
#endif
      }
      else {
        throw(vpException(vpException::functionNotImplementedError, "Currently, only initialization by click is handled."));
      }
      RCLCPP_DEBUG(this->get_logger(), "Done init");
    }

    double t_start = vpTime::measureTimeMs();
    vpRBTrackingResult result;
    if (m_depth_is_required) {
      result = m_tracker.track(m_I, m_Ic, m_I_depth);
    }
    else {
      result = m_tracker.track(m_I, m_Ic);
    }
    double t_end_tracking = vpTime::measureTimeMs();
    static const unsigned int nb_digits = 2; // Number of digits to display doubles on screen
    std::string t_string = std::to_string(t_end_tracking - t_start);
    std::string tracking_time = "Tracking time: " + t_string.substr(0, t_string.find(".") + nb_digits + 1) + "ms";
    RCLCPP_DEBUG_STREAM(this->get_logger(), tracking_time);

    if ((result.getStoppingReason() == vpRBTrackingStoppingReason::CONVERGENCE_CRITERION) || (result.getStoppingReason() == vpRBTrackingStoppingReason::MAX_ITERS)) {
      m_tracker.getPose(cMo);
      RCLCPP_DEBUG_STREAM(this->get_logger(), "c_M_o:= [ " << cMo.getTranslationVector().t() << " ] m [ " << vpThetaUVector(cMo.getRotationMatrix()).t() << " ] rad");
    }
    else {
      switch (result.getStoppingReason()) {
      case vpRBTrackingStoppingReason::EXCEPTION:
      {
        RCLCPP_WARN_STREAM(this->get_logger(), "Encountered an exception during tracking, pose was not updated!" << std::endl);
        break;
      }
      case vpRBTrackingStoppingReason::NOT_ENOUGH_FEATURES:
      {
        RCLCPP_WARN_STREAM(this->get_logger(), "There were not enough feature to perform tracking!" << std::endl);
        break;
      }
      case vpRBTrackingStoppingReason::OBJECT_NOT_IN_IMAGE:
      {
        RCLCPP_WARN_STREAM(this->get_logger(), "Object is not in image!" << std::endl);
        break;
      }
      default:
      { }
      }
      m_tracker_initialized = false;
    }

    if (m_tracker_initialized) {
       // Publish the poses for display on a remote GUI if headless mode is active
      visp_tracker_common::msg::NamedPoseArray poseArrayMsg;

      geometry_msgs::msg::PoseStamped pose_c_M_o;
      pose_c_M_o.pose = std::move(visp_common::pose::toGeometryMsgsPose(cMo));
      pose_c_M_o.header.frame_id = m_frame_id;
      pose_c_M_o.header.stamp = this->get_clock()->now();
      visp_tracker_common::msg::NamedPose namedPoseMsg_c_M_o;
      namedPoseMsg_c_M_o.name = "c_M_o";
      namedPoseMsg_c_M_o.pose = pose_c_M_o;
      poseArrayMsg.poses.push_back(namedPoseMsg_c_M_o);

      m_poses_pub->publish(poseArrayMsg);

      // Fill info strings
      std::vector<std::string> vec_info; // Vector that contains info to display on screen
      vec_info.push_back(tracking_time);
      {
        auto drift_detector = m_tracker.getDriftDetector();
        if (drift_detector) {
          std::stringstream ss;
          ss << "Drift score: " << drift_detector->getScore();
          vec_info.push_back(ss.str());
        }
      }

      // Manage info strings publication
      if (m_info_strings.info_strings.size() == m_info_nb_static) {
        m_info_strings.info_strings.insert(m_info_strings.info_strings.end(), vec_info.begin(), vec_info.end());
      }
      else {
        unsigned int nb_infos = vec_info.size();
        m_info_strings.info_strings.resize(m_info_nb_static + nb_infos);
        for (unsigned int i = 0; i < nb_infos; ++i) {
          m_info_strings.info_strings[m_info_nb_static + i] = vec_info[i];
        }
      }
      m_info_strings.hor_offset_right_border.resize(m_info_strings.info_strings.size(), 1.5 * BaseTracker::s_default_hor_offset);
      m_info_strings_pub->publish(m_info_strings);

      // Publish the model
      if (m_visualization_debug && m_is_headless_mode) {
        const std::vector<vpRBSilhouettePoint> &silhouettePts = m_tracker.getMostRecentFrame().silhouettePoints;
        const std::string name = "Object silhouette";
        visp_tracker_common::msg::NamedFeature features_msg;
        for (const auto &silhouettePt: silhouettePts) {
          features_msg.image_points.push_back(vision_msgs::msg::Point2D().set__x(silhouettePt.j).set__y(silhouettePt.i));
        }
        visp_tracker_common::msg::NamedFeatureArray features_array;
        features_array.features.push_back(features_msg);
        m_features_pub->publish(features_array);
      }
    }
  }

#if defined(VISP_HAVE_DISPLAY) && defined(VISP_HAVE_MODULE_GUI)
  if (display_frame) {
    if (m_tracker_initialized && m_has_to_track) {
      m_tracker.display(m_I, m_Ic, m_I_depth_display);
      vpDisplay::displayFrame(m_Ic, cMo, m_rgb_cam, 0.01);
    }
    unsigned int nb_infos = m_info_strings.info_strings.size();
    const unsigned int v_offset = 20;
    for (unsigned int r = 0; r < nb_infos; ++r) {
      vpDisplay::displayText(m_I, v_offset * (r + 1), m_I.getWidth() - m_info_strings.hor_offset_right_border[r], m_info_strings.info_strings[r], vpColor::red);
    }
    vpDisplay::flush(m_I);
    vpDisplay::flush(m_Ic);
    vpDisplay::flush(m_I_depth_display);
  }

  if (!m_is_headless_mode) {
    vpMouseButton::vpMouseButtonType button;
    if (vpDisplay::getClick(m_Ic, button, false)) {
      switch (button) {
      case vpMouseButton::button1:
      {
        std::scoped_lock lock(m_mutex_tracking);
        m_has_to_track = (!m_has_to_track);
        break;
      }
      case vpMouseButton::button3:
      {
        {
          std::scoped_lock lock(m_mutex_tracking);
          m_has_to_track = false;
        }
        {
          std::scoped_lock lock(m_mutex_quit);
          m_quit = true;
        }
        break;
      }
      default:
        break;
      }
    }
  }
#endif
}

}
