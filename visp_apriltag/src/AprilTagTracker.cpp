#include <visp_apriltag/AprilTagTracker.hpp>

namespace visp_apriltag
{
AprilTagTracker::AprilTagTracker(const std::string &node_name)
  : visp_tracker_common::BaseTracker(node_name, true)
{

}

//////////////////////////////////////////////////////////////////////
//                        INITIALIZATION                            //
//////////////////////////////////////////////////////////////////////

bool AprilTagTracker::init_tracker() { return true; }
void AprilTagTracker::init_info_strings() { }

//////////////////////////////////////////////////////////////////////
//                        ROS2 SUBCRIPTIONS                         //
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//                        OTHERS                                    //
//////////////////////////////////////////////////////////////////////

std::string AprilTagTracker::tagFamilyToString(const vpDetectorAprilTag::vpAprilTagFamily &type)
{
  std::string name;
  switch (type) {
  case vpDetectorAprilTag::TAG_36h11:
    name = "36h11";
    break;
  case vpDetectorAprilTag::TAG_36h10:
    name = "36h10";
    break;
  case vpDetectorAprilTag::TAG_36ARTOOLKIT:
    name = "36artoolkit";
    break;
  case vpDetectorAprilTag::TAG_25h9:
    name = "25h9";
    break;
  case vpDetectorAprilTag::TAG_25h7:
    name = "25h7";
    break;
  case vpDetectorAprilTag::TAG_16h5:
    name = "16h5";
    break;
  case vpDetectorAprilTag::TAG_CIRCLE21h7:
    name = "circle21h7";
    break;
  case vpDetectorAprilTag::TAG_CIRCLE49h12:
    name = "circle49h12";
    break;
  case vpDetectorAprilTag::TAG_CUSTOM48h12:
    name = "custom48h12";
    break;
  case vpDetectorAprilTag::TAG_STANDARD41h12:
    name = "standard41h12";
    break;
  case vpDetectorAprilTag::TAG_STANDARD52h13:
    name = "standard52h13";
    break;
  case vpDetectorAprilTag::TAG_ARUCO_4x4_50:
    name = "aruco_4x4_50";
    break;
  case vpDetectorAprilTag::TAG_ARUCO_4x4_100:
    name = "aruco_4x4_100";
    break;
  case vpDetectorAprilTag::TAG_ARUCO_4x4_250:
    name = "aruco_4x4_250";
    break;
  case vpDetectorAprilTag::TAG_ARUCO_4x4_1000:
    name = "aruco_4x4_1000";
    break;
  case vpDetectorAprilTag::TAG_ARUCO_5x5_50:
    name = "aruco_5x5_50";
    break;
  case vpDetectorAprilTag::TAG_ARUCO_5x5_100:
    name = "aruco_5x5_100";
    break;
  case vpDetectorAprilTag::TAG_ARUCO_5x5_250:
    name = "aruco_5x5_250";
    break;
  case vpDetectorAprilTag::TAG_ARUCO_5x5_1000:
    name = "aruco_5x5_1000";
    break;
  case vpDetectorAprilTag::TAG_ARUCO_6x6_50:
    name = "aruco_6x6_50";
    break;
  case vpDetectorAprilTag::TAG_ARUCO_6x6_100:
    name = "aruco_6x6_100";
    break;
  case vpDetectorAprilTag::TAG_ARUCO_6x6_250:
    name = "aruco_6x6_250";
    break;
  case vpDetectorAprilTag::TAG_ARUCO_6x6_1000:
    name = "aruco_6x6_1000";
    break;
  case vpDetectorAprilTag::TAG_ARUCO_7x7_50:
    name = "aruco_7x7_50";
    break;
  case vpDetectorAprilTag::TAG_ARUCO_7x7_100:
    name = "aruco_7x7_100";
    break;
  case vpDetectorAprilTag::TAG_ARUCO_7x7_250:
    name = "aruco_7x7_250";
    break;
  case vpDetectorAprilTag::TAG_ARUCO_7x7_1000:
    name = "aruco_7x7_1000";
    break;
  case vpDetectorAprilTag::TAG_ARUCO_MIP_36h12:
    name = "aruco_mip_36h12";
    break;
  default:
    name = "unknown";
  }
  return name;
}

vpDetectorAprilTag::vpAprilTagFamily AprilTagTracker::tagFamilyFromString(const std::string &name)
{
  vpDetectorAprilTag::vpAprilTagFamily res = vpDetectorAprilTag::vpAprilTagFamily::TAG_ARUCO_MIP_36h12;
  bool wasFound = false;
  std::string lowerCaseName = vpIoTools::toLowerCase(name);
  unsigned int i = 0;
  while ((i <= vpDetectorAprilTag::vpAprilTagFamily::TAG_ARUCO_MIP_36h12) && (!wasFound)) {
    vpDetectorAprilTag::vpAprilTagFamily candidate = static_cast<vpDetectorAprilTag::vpAprilTagFamily>(i);
    if (lowerCaseName == tagFamilyToString(candidate)) {
      res = candidate;
      wasFound = true;
    }
    ++i;
  }
  if (!wasFound) {
    throw(vpException(vpException::badValue, "Could not find a tag family that corresponds to the name '%s'", name.c_str()));
  }
  return res;
}

std::string AprilTagTracker::getAvailableTagFamily(const std::string &prefix, const std::string &sep, const std::string &suffix)
{
  std::string modes(prefix);
  for (unsigned int i = 0; i < vpDetectorAprilTag::vpAprilTagFamily::TAG_ARUCO_MIP_36h12; ++i) {
    vpDetectorAprilTag::vpAprilTagFamily candidate = static_cast<vpDetectorAprilTag::vpAprilTagFamily>(i);
    modes += tagFamilyToString(candidate) + sep;
  }
  vpDetectorAprilTag::vpAprilTagFamily candidate = static_cast<vpDetectorAprilTag::vpAprilTagFamily>(vpDetectorAprilTag::vpAprilTagFamily::TAG_ARUCO_MIP_36h12);
  modes += tagFamilyToString(candidate) + suffix;
  return modes;
}

std::string AprilTagTracker::poseMethodToString(const vpDetectorAprilTag::vpPoseEstimationMethod &method)
{
  std::string name;
  switch (method) {
  case vpDetectorAprilTag::HOMOGRAPHY:
    name = "homography";
    break;
  case vpDetectorAprilTag::HOMOGRAPHY_VIRTUAL_VS:
    name = "homography_virtual_vs";
    break;
  case vpDetectorAprilTag::DEMENTHON_VIRTUAL_VS:
    name = "dementhon_virtual_vs";
    break;
  case vpDetectorAprilTag::LAGRANGE_VIRTUAL_VS:
    name = "lagrange_virtual_vs";
    break;
  case vpDetectorAprilTag::BEST_RESIDUAL_VIRTUAL_VS:
    name = "best_residual_virtual_vs";
    break;
  case vpDetectorAprilTag::HOMOGRAPHY_ORTHOGONAL_ITERATION:
    name = "homography_orthogonal_iteration";
    break;
  default:
    name = "unknown";
  }
  return name;
}

vpDetectorAprilTag::vpPoseEstimationMethod AprilTagTracker::poseMethodFromString(const std::string &name)
{
  vpDetectorAprilTag::vpPoseEstimationMethod res = vpDetectorAprilTag::vpPoseEstimationMethod::HOMOGRAPHY_ORTHOGONAL_ITERATION;
  bool wasFound = false;
  std::string lowerCaseName = vpIoTools::toLowerCase(name);
  unsigned int i = 0;
  while ((i <= vpDetectorAprilTag::vpPoseEstimationMethod::HOMOGRAPHY_ORTHOGONAL_ITERATION) && (!wasFound)) {
    vpDetectorAprilTag::vpPoseEstimationMethod candidate = static_cast<vpDetectorAprilTag::vpPoseEstimationMethod>(i);
    if (lowerCaseName == poseMethodToString(candidate)) {
      res = candidate;
      wasFound = true;
    }
    ++i;
  }
  if (!wasFound) {
    throw(vpException(vpException::badValue, "Could not find a pose estimation method that corresponds to the name '%s'", name.c_str()));
  }
  return res;
}

std::string AprilTagTracker::getAvailablePoseMethod(const std::string &prefix, const std::string &sep, const std::string &suffix)
{
  std::string modes(prefix);
  for (unsigned int i = 0; i < vpDetectorAprilTag::vpPoseEstimationMethod::HOMOGRAPHY_ORTHOGONAL_ITERATION; ++i) {
    vpDetectorAprilTag::vpPoseEstimationMethod candidate = static_cast<vpDetectorAprilTag::vpPoseEstimationMethod>(i);
    modes += poseMethodToString(candidate) + sep;
  }
  vpDetectorAprilTag::vpPoseEstimationMethod candidate = static_cast<vpDetectorAprilTag::vpPoseEstimationMethod>(vpDetectorAprilTag::vpPoseEstimationMethod::HOMOGRAPHY_ORTHOGONAL_ITERATION);
  modes += poseMethodToString(candidate) + suffix;
  return modes;
}


}
