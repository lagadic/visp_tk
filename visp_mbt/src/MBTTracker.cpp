#include <visp_mbt/MBTTracker.hpp>

namespace visp_mbt
{
MBTTracker::MBTTracker(const std::string &name) : visp_tracker_common::BaseMultiModalTracker(name, true)
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

  auto max_z_param = rclcpp::Parameter();
  auto max_z_param_desc = rcl_interfaces::msg::ParameterDescriptor {};
  max_z_param_desc.description = "This parameter permits the maximum depth we want to display.";
  this->declare_parameter("max_z_display", 2.0, max_z_param_desc);
  this->get_parameter("max_z_display", max_z_param);
  m_max_z_display = max_z_param.as_double();

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

bool MBTTracker::init_tracker()
{
  // Retrieving init file
  m_init_file_path = visp_common::path::path_retriever(m_init_file_path);
  if (m_init_file_path.empty()) {
    RCLCPP_ERROR(this->get_logger(), "init file path is empty");
    return false;
  }
  else if (!vpIoTools::checkFilename(m_init_file_path)) {
    RCLCPP_ERROR(this->get_logger(), "init file %s does not exist", m_init_file_path.c_str());
    return false;
  }
  RCLCPP_INFO(this->get_logger(), "MBT will use the init file %s", m_init_file_path.c_str());

  // Retrieving config file
  std::string config_file_path = this->get_parameter("config_file").as_string();
  config_file_path = visp_common::path::path_retriever(config_file_path);

  if (config_file_path.empty()) {
    RCLCPP_ERROR(this->get_logger(), "config file path is empty");
    return false;
  }
  else if (!vpIoTools::checkFilename(config_file_path)) {
    RCLCPP_ERROR(this->get_logger(), "config file %s does not exist", config_file_path.c_str());
    return false;
  }
  RCLCPP_INFO(this->get_logger(), "MBT will use the config file %s", config_file_path.c_str());

  if (config_file_path.find(".xml") != std::string::npos) {
    return init_from_xml();
  }
  else if (config_file_path.find(".json") != std::string::npos) {
    return init_from_json();
  }
  else {
    RCLCPP_ERROR(this->get_logger(), "config file %s is neither an XML file nor a JSON file", config_file_path.c_str());
    return false;
  }
}

bool MBTTracker::init_from_xml()
{
///TODO: types are mandatory
// std::map<std::string, int> map_name_types;
// map_name_types["Color"] = vpMbGenericTracker::EDGE_TRACKER | vpMbGenericTracker::KLT_TRACKER;
// map_name_types["Depth"] = vpMbGenericTracker::DEPTH_NORMAL_TRACKER;
// m_tracker.setTrackerType(map_name_types);

///TODO: model(s) is/are mandatory
  throw(vpException(vpException::notImplementedError, "XML initialization is not implemented yet"));
  return true;
}

bool MBTTracker::init_from_json()
{
  ///TODO: read JSON and parse potential package:// used in the model sections
  ///TODO: read config file
  ///TOODO: if model(s) is/are not in the model sections, model attribute is mandatory
  throw(vpException(vpException::notImplementedError, "JSON initialization is not implemented yet"));
  return true;
}

void MBTTracker::init_info_strings() { }

void MBTTracker::check_requires_depth()
{
  const auto typemap = m_tracker.getCameraTrackerTypes();
  m_depth_is_required = false;
  for (const auto &pair : typemap) {
    if (pair.second & vpMbGenericTracker::EDGE_TRACKER || pair.second & vpMbGenericTracker::KLT_TRACKER) {
      m_color_trackers_name.push_back(pair.first);
    }
    else if (pair.second & vpMbGenericTracker::DEPTH_DENSE_TRACKER || pair.second & vpMbGenericTracker::DEPTH_NORMAL_TRACKER) {
      m_depth_trackers_name.push_back(pair.first);
      m_depth_is_required = true;
    }
  }
}

//////////////////////////////////////////////////////////////////////
//                        OTHERS                                    //
//////////////////////////////////////////////////////////////////////
void MBTTracker::treat_rgb(const sensor_msgs::msg::Image::ConstSharedPtr &rgb)
{
  if ((!m_rgb_cam_info_received) || (m_depth_is_required && !m_depth_cam_info_received)) {
    return;
  }

  m_Ic = std::move(visp_common::image::toVispImageRGBa(*rgb));

  vpImageConvert::convert(m_Ic, m_I);
}

void MBTTracker::treat_depth(const sensor_msgs::msg::Image::ConstSharedPtr &depth)
{
  if (!m_depth_cam_info_received) {
    return;
  }

  // Convert ROS2 image to ViSP image
  const vpImage<uint16_t> &I_depth_raw = visp_common::image::toVispImageUint16(*depth);

  static bool once = true;
  if (once) {
    if (!m_is_headless_mode) {
      // Initialize depth display
      m_I_depth_display.resize(I_depth_raw.getHeight(), I_depth_raw.getWidth());
      m_display_depth = vpDisplayFactory::createDisplay(m_I_depth_display);
    }

    for (auto name: m_depth_trackers_name) {
      // Initialize size of the point clouds
      m_map_pcw[name] = I_depth_raw.getWidth();
      m_map_pch[name] = I_depth_raw.getHeight();
    }

    once = false;
  }

  // Clear the point cloud
  m_pointcloud.resize(I_depth_raw.getSize(), vpColVector(3, 0.));
  static const double Z_factor = 0.001;
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
      double Z = Z_raw * Z_factor;
      if ((Z > 0.) && (!std::isnan(Z_raw)) && std::isfinite(Z)) {
        double x = 0.0, y = 0.0;
        vpPixelMeterConversion::convertPoint(m_depth_cam, j, i, x, y);
        m_pointcloud[idx][0] = x * Z;
        m_pointcloud[idx][1] = y * Z;
        m_pointcloud[idx][2] = Z;
        if (!m_is_headless_mode) {
          m_I_depth_display.bitmap[idx] = Z > m_max_z_display ? 0 : static_cast<unsigned int>((Z / m_max_z_display) * 255.f);
        }
      }
      else if (!m_is_headless_mode) {
        m_I_depth_display.bitmap[idx] = 0;
      }
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

void MBTTracker::track()
{
  bool quit = false;
  {
    std::scoped_lock lock(m_mutex_quit);
    quit = m_quit;
  }

  if (quit || (!m_rgb_cam_info_received) || ((!m_depth_cam_info_received) && m_depth_is_required)) {
    return;
  }

  if (!m_tracker_cams_set) {
    if (!m_depth_is_required) {
      RCLCPP_DEBUG(this->get_logger(), "Setting the tracker camera parameters using the color camera intrinsics only");
      m_tracker.setCameraParameters(m_rgb_cam);
    }
    else {
      RCLCPP_DEBUG(this->get_logger(), "Setting the tracker camera parameters using the depth and color camera intrinsics");
      m_tracker.setCameraParameters(m_rgb_cam, m_depth_cam);
    }
    m_tracker_cams_set = true;
  }

  // Check if frame has to be displayed
  bool display_frame = ((!m_is_headless_mode) || (!m_tracker_initialized)) &&((m_display_nb_frames_skipped <= 0) || ((m_frame_cnt % m_display_nb_frames_skipped) == 0));

  if (display_frame) {

    if (!m_display_initialized) {
      m_display = vpDisplayFactory::createDisplay(m_Ic);
      m_display_initialized = true;
    }

    vpDisplay::display(m_Ic);
    vpDisplay::display(m_I_depth_display);
    {
      std::stringstream ss;
      ss << "Left click to " << (m_has_to_track ? "stop" : "start") << " the tracking, right click to quit.";
      vpDisplay::displayText(m_Ic, 20, 20, ss.str(), vpColor::red);
    }
  }

  vpHomogeneousMatrix cMo;

  if (!m_tracker_initialized) {
    m_tracker.initClick(m_Ic, m_init_file_path, true);
    m_tracker.getPose(cMo);
    m_tracker_initialized = true;
  }

  double t_start = vpTime::measureTimeMs();
  try {
    if (m_depth_is_required) {
      for (auto name: m_color_trackers_name) {
        m_map_img[name] = &m_Ic;
      }

      for (auto name: m_depth_trackers_name) {
        m_map_pc[name] = &m_pointcloud;
      }

      m_tracker.track(m_map_img, m_map_pc, m_map_pcw, m_map_pch);
    }
    else {
      m_tracker.track(m_Ic);
    }
    double t_end_tracking = vpTime::measureTimeMs();
    RCLCPP_DEBUG_STREAM(this->get_logger(), "Tracking time: " << (t_end_tracking - t_start) << "ms");

    m_tracker.getPose(cMo);
    RCLCPP_DEBUG_STREAM(this->get_logger(), "c_M_o:= [ " << cMo.getTranslationVector().t() << " ] m [ " << vpThetaUVector(cMo.getRotationMatrix()).t() << " ] rad");
  }
  catch (vpTrackingException &e) {
    RCLCPP_WARN(this->get_logger(), "Tracking failed. Reason: %s", e.getMessage());
    m_tracker_initialized = false;
  }
  catch (vpException &e) {
    RCLCPP_ERROR(this->get_logger(), "Got unexpected error: %s", e.getMessage());
    throw;
  }

  if (m_tracker_initialized) {
     // Publish the poses for display on a remote GUI if headless mode is active
    visp_tracker_common::msg::NamedPoseArray poseArrayMsg;

    geometry_msgs::msg::PoseStamped pose_c_M_o;
    pose_c_M_o.pose = std::move(visp_common::pose::toGeometryMsgsPose(cMo));
    visp_tracker_common::msg::NamedPose namedPoseMsg_c_M_o;
    namedPoseMsg_c_M_o.name = "c_M_o";
    namedPoseMsg_c_M_o.pose = pose_c_M_o;
    poseArrayMsg.poses.push_back(namedPoseMsg_c_M_o);

    m_poses_pub->publish(poseArrayMsg);

    // Publish the model
    if (m_visualization_debug && m_is_headless_mode) {
      const auto &model = m_tracker.getModelForDisplay(m_Ic.getWidth(), m_Ic.getHeight(), cMo, m_rgb_cam);
      const std::string name = "Current MBT model";
      const visp_tracker_common::msg::NamedFeature &features_msg = mbtModelToMsg(model, name, this->get_logger());
      visp_tracker_common::msg::NamedFeatureArray features_array;
      features_array.features.push_back(features_msg);
      m_features_pub->publish(features_array);
    }
  }

  if (display_frame) {
    if (m_tracker_initialized) {
      m_tracker.display(m_Ic, cMo, m_rgb_cam, vpColor::red, 1, false);
      std::stringstream ss;
      ss << "Features: edges " << m_tracker.getNbFeaturesEdge() << ", klt " << m_tracker.getNbFeaturesKlt();
      ss << ", depth dense " << m_tracker.getNbFeaturesDepthDense() << ", depth normal " << m_tracker.getNbFeaturesDepthNormal();
      vpDisplay::displayText(m_Ic, (m_display->getHeight() - 25)*m_display->getDownScalingFactor(), 20, ss.str(), vpColor::red);
      vpDisplay::displayFrame(m_Ic, cMo, m_rgb_cam, 0.01);
    }
    vpDisplay::flush(m_Ic);
    vpDisplay::flush(m_I_depth_display);

    if (m_is_headless_mode) {
      vpDisplay::close(m_Ic);
      m_display.reset();
      m_display_initialized = false;
    }
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
}

visp_tracker_common::msg::NamedFeature MBTTracker::mbtModelToMsg(const std::vector<std::vector<double>> &model, const std::string &name, const rclcpp::Logger &logger)
{
  visp_tracker_common::msg::NamedFeature features_msg;
  features_msg.name = name;
  for (const auto &feature: model) {
    if (vpMath::equal(feature[0], 0.)) {
      visp_tracker_common::msg::Point2DTuple line;
      line.start.x = feature[2];
      line.start.y = feature[1];
      line.end.x = feature[4];
      line.end.y = feature[3];
      features_msg.lines.push_back(line);
    }
    else if (vpMath::equal(feature[0], 1.)) {
      visp_tracker_common::msg::Ellipse2D ellipse;
      ellipse.center.x = feature[2];
      ellipse.center.y = feature[1];
      ellipse.n20 = feature[3];
      ellipse.n11 = feature[4];
      ellipse.n02 = feature[5];
      features_msg.ellipses.push_back(ellipse);
    }
    else {
      RCLCPP_WARN_STREAM(logger, "Primitive of type " << feature[0] << " is not handled yet when converting a MBT model into a ROS2 message.");
    }
  }
  return features_msg;
}

}
