/****************************************************************************
 *
 * ViSP, open source Visual Servoing Platform software.
 * Copyright (C) 2005 - 2026 by Inria. All rights reserved.
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
 * Inria Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *****************************************************************************/

#ifndef VISP_TRACKER_COMMON_DISPLAY_HPP
#define VISP_TRACKER_COMMON_DISPLAY_HPP

#include <visp_tracker_common/msg/named_feature.hpp>

#include <visp3/core/vpConfig.h>
#include <visp3/core/vpColor.h>
#include <visp3/core/vpDisplay.h>
#include <visp3/core/vpImage.h>
#include <visp3/core/vpImagePoint.h>

#include <vector>

namespace visp_tracker_common
{
namespace display
{
/**
 * @brief How the image points of a 2D feature are rendered.
 */
enum class PointsType
{
  POINT, //!< A point
  CROSS  //!< A cross
};

/**
 * @brief Display a 2D feature in an image that has a display: its image points, ellipses, lines, polygons and
 * rectangles. The name of the feature is not displayed.
 *
 * @tparam Type Type of the image pixels (unsigned char or vpRGBa).
 * @param I Image in which the feature is displayed.
 * @param feature Feature to display.
 * @param color Color of the feature.
 * @param thickness Thickness of the drawings.
 * @param points_type How the image points are rendered.
 */
template <typename Type>
void displayNamedFeature(const VISP_NAMESPACE_ADDRESSING vpImage<Type> &I, const visp_tracker_common::msg::NamedFeature &feature,
                         const VISP_NAMESPACE_ADDRESSING vpColor &color, unsigned int thickness, PointsType points_type)
{
  using VISP_NAMESPACE_ADDRESSING vpDisplay;
  using VISP_NAMESPACE_ADDRESSING vpImagePoint;

  for (const auto &ip : feature.image_points) {
    if (points_type == PointsType::CROSS) {
      vpDisplay::displayCross(I, ip.y, ip.x, 10, color, thickness);
    }
    else {
      vpDisplay::displayPoint(I, ip.y, ip.x, color, thickness);
    }
  }

  for (const auto &ellipse : feature.ellipses) {
    vpDisplay::displayEllipse(I, vpImagePoint(ellipse.center.y, ellipse.center.x), ellipse.n20, ellipse.n11, ellipse.n02, true, color, thickness);
  }

  for (const auto &line : feature.lines) {
    vpDisplay::displayLine(I, vpImagePoint(line.start.y, line.start.x), vpImagePoint(line.end.y, line.end.x), color, thickness);
  }

  for (const auto &poly : feature.polygons) {
    if (poly.lines.empty()) {
      continue;
    }
    std::vector<vpImagePoint> poly_points;
    poly_points.push_back(vpImagePoint(poly.lines[0].start.y, poly.lines[0].start.x));
    for (const auto &line : poly.lines) {
      poly_points.push_back(vpImagePoint(line.end.y, line.end.x));
    }
    vpDisplay::displayPolygon(I, poly_points, color, thickness);
  }

  for (const auto &rect : feature.rectangles) {
    vpDisplay::displayRectangle(I, vpImagePoint(rect.start.y, rect.start.x), vpImagePoint(rect.end.y, rect.end.x), color, false, thickness);
  }
}
} // namespace display
} // namespace visp_tracker_common

#endif
