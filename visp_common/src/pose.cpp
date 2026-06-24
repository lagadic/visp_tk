/*
 * Copyright (C) 2026 by Inria. All rights reserved.
 *
 * This software is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * See the file LICENSE.txt at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * For using this software that can not be combined with the GNU
 * GPL, or if you have questions regarding the use of this file,
 * please contact Inria at visp@inria.fr
 *
 * This software was developed at:
 * Inria centre at Rennes University
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*!
  \file pose.cpp
  \brief conversions between ROS and ViSP structures representing a 3D pose
*/

#include <cmath>

#include <visp3/core/vpQuaternionVector.h>
#include <visp3/core/vpRotationMatrix.h>
#include <visp3/core/vpTranslationVector.h>

#include <visp_common/pose.hpp>

namespace visp_common
{
namespace pose
{
#ifdef ENABLE_VISP_NAMESPACE
using namespace VISP_NAMESPACE_NAME;
#endif

vpHomogeneousMatrix toVispHomogeneousMatrix(const geometry_msgs::msg::Pose &pose)
{
  vpHomogeneousMatrix mat;
  vpTranslationVector vec(pose.position.x, pose.position.y, pose.position.z);
  vpQuaternionVector q(pose.orientation.x, pose.orientation.y, pose.orientation.z, pose.orientation.w);
  mat.buildFrom(vec, q);

  return mat;
}

vpHomogeneousMatrix toVispHomogeneousMatrix(const geometry_msgs::msg::Pose::ConstSharedPtr &pose)
{
  vpHomogeneousMatrix mat;
  vpTranslationVector vec(pose->position.x, pose->position.y, pose->position.z);
  vpQuaternionVector q(pose->orientation.x, pose->orientation.y, pose->orientation.z, pose->orientation.w);
  mat.buildFrom(vec, q);
  return mat;
}

vpHomogeneousMatrix toVispHomogeneousMatrix(const geometry_msgs::msg::Transform &trans)
{
  vpHomogeneousMatrix mat;
  vpTranslationVector vec(trans.translation.x, trans.translation.y, trans.translation.z);
  vpQuaternionVector q(trans.rotation.x, trans.rotation.y, trans.rotation.z, trans.rotation.w);
  mat.buildFrom(vec, q);
  return mat;
}

vpHomogeneousMatrix toVispHomogeneousMatrix(const geometry_msgs::msg::Transform::ConstSharedPtr &trans)
{
  vpHomogeneousMatrix mat;
  vpTranslationVector vec(trans->translation.x, trans->translation.y, trans->translation.z);
  vpQuaternionVector q(trans->rotation.x, trans->rotation.y, trans->rotation.z, trans->rotation.w);
  mat.buildFrom(vec, q);
  return mat;
}

geometry_msgs::msg::Transform toGeometryMsgsTransform(const vpHomogeneousMatrix &mat)
{
  geometry_msgs::msg::Transform trans;
  vpQuaternionVector q;
  mat.extract(q);
  trans.rotation.x = q.x();
  trans.rotation.y = q.y();
  trans.rotation.z = q.z();
  trans.rotation.w = q.w();

  trans.translation.x = mat[0][3];
  trans.translation.y = mat[1][3];
  trans.translation.z = mat[2][3];

  return trans;
}

geometry_msgs::msg::Pose toGeometryMsgsPose(const vpHomogeneousMatrix &mat)
{
  geometry_msgs::msg::Pose pose;

  vpThetaUVector tu(mat);
  vpColVector u;
  double theta;
  tu.extract(theta, u);

  theta *= 0.5;

  double sinTheta_2 = sin(theta);

  pose.orientation.x = u[0] * sinTheta_2;
  pose.orientation.y = u[1] * sinTheta_2;
  pose.orientation.z = u[2] * sinTheta_2;
  pose.orientation.w = cos(theta);

  pose.position.x = mat[0][3];
  pose.position.y = mat[1][3];
  pose.position.z = mat[2][3];

  return pose;
}
} // namespace pose
} // namespace visp_common
