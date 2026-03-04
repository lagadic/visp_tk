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
  \file image.cpp
  \brief Implements conversions between ViSP and ROS image types
 */

#include <stdexcept>

#include <sensor_msgs/image_encodings.hpp>
#include <sensor_msgs/msg/image.hpp>

#include <visp_common/image.hpp>
#include <visp3/core/vpException.h>

#include <visp3/core/vpConfig.h>
#ifdef VISP_HAVE_OPENMP
#include <omp.h>
#endif

namespace visp_common
{
namespace image
{
sensor_msgs::msg::Image toSensorMsgsImage(const vpImage<unsigned char> &src)
{
  sensor_msgs::msg::Image dst;
  dst.width = src.getWidth();
  dst.height = src.getHeight();
  dst.encoding = sensor_msgs::image_encodings::MONO8;
  dst.step = src.getWidth();
  dst.data.resize(dst.height * dst.step);
  memcpy(&dst.data[0], src.bitmap, dst.height * dst.step * sizeof(unsigned char));

  return dst;
}

sensor_msgs::msg::Image toSensorMsgsImage(const vpImage<uint16_t> &src)
{
  sensor_msgs::msg::Image dst;
  dst.width = src.getWidth();
  dst.height = src.getHeight();
  dst.encoding = sensor_msgs::image_encodings::MONO16;
  dst.step = src.getWidth() * sizeof(uint16_t);
  dst.data.resize(dst.height * dst.step);
  memcpy(&dst.data[0], src.bitmap, dst.height * dst.step);

  return dst;
}

sensor_msgs::msg::Image toSensorMsgsImage(const vpImage<vpRGBa> &src)
{
  sensor_msgs::msg::Image dst;
  dst.width = src.getWidth();
  dst.height = src.getHeight();
  dst.encoding = sensor_msgs::image_encodings::RGB8;
  unsigned nc = sensor_msgs::image_encodings::numChannels(dst.encoding);
  dst.step = src.getWidth() * nc;

  dst.data.resize(dst.height * dst.step);
  int width = src.getWidth();
  int size = src.getSize();
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
      dst.data[i * dst.step + j * nc + 0] = src.bitmap[i * src.getWidth() + j].R;
      dst.data[i * dst.step + j * nc + 1] = src.bitmap[i * src.getWidth() + j].G;
      dst.data[i * dst.step + j * nc + 2] = src.bitmap[i * src.getWidth() + j].B;
      // dst.data[i * dst.step + j * nc + 3] = src.bitmap[i * dst.step + j].A;
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
  return dst;
}

vpImage<unsigned char> toVispImageChar(const sensor_msgs::msg::Image &src)
{
  using sensor_msgs::image_encodings::BGR8;
  using sensor_msgs::image_encodings::BGRA8;
  using sensor_msgs::image_encodings::MONO8;
  using sensor_msgs::image_encodings::RGB8;
  using sensor_msgs::image_encodings::RGBA8;

  vpImage<unsigned char> dst(src.height, src.width);

  if (src.encoding == sensor_msgs::image_encodings::MONO8) {
    memcpy(dst.bitmap, &(src.data[0]), dst.getHeight() * src.step * sizeof(unsigned char));
  }
  else if (src.encoding == sensor_msgs::image_encodings::RGB8 || src.encoding == RGBA8 ||
           src.encoding == sensor_msgs::image_encodings::BGR8 || src.encoding == sensor_msgs::image_encodings::BGRA8) {
    unsigned nc = sensor_msgs::image_encodings::numChannels(src.encoding);
    unsigned cEnd = (src.encoding == RGBA8 || src.encoding == sensor_msgs::image_encodings::BGRA8) ? nc - 1 : nc;

    int width = dst.getWidth();
    int size = dst.getSize();
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
        int acc = 0;
        for (unsigned c = 0; c < cEnd; ++c) {
          acc += src.data[i * src.step + j * nc + c];
        }
        dst.bitmap[idx] = acc / nc;
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
  else {
    throw(vpException(vpException::fatalError, "Format %s can be converted into vpImage<uchar>", src.encoding));
  }
  return dst;
}

vpImage<uint16_t> toVispImageUint16(const sensor_msgs::msg::Image &src)
{
  if ((src.encoding != sensor_msgs::image_encodings::MONO16) && (src.encoding != sensor_msgs::image_encodings::TYPE_16UC1)) {
    throw(vpException(vpException::fatalError, "Only %s and %s can be converted into vpImage<uint16_t>", sensor_msgs::image_encodings::MONO16, sensor_msgs::image_encodings::TYPE_16UC1));
  }
  vpImage<uint16_t> Ivisp(src.height, src.width);
  memcpy(Ivisp.bitmap, &(src.data[0]), src.height * src.width * sizeof(uint16_t));
  return Ivisp;
}

vpImage<vpRGBa> toVispImageRGBa(const sensor_msgs::msg::Image &src)
{
  using sensor_msgs::image_encodings::BGR8;
  using sensor_msgs::image_encodings::BGRA8;
  using sensor_msgs::image_encodings::MONO8;
  using sensor_msgs::image_encodings::RGB8;
  using sensor_msgs::image_encodings::RGBA8;

  vpImage<vpRGBa> dst(src.height, src.width);

  if (src.encoding == sensor_msgs::image_encodings::MONO8) {
    int width = dst.getWidth();
    int size = dst.getSize();
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
        dst.bitmap[idx] = vpRGBa(src.data[i * src.step + j], src.data[i * src.step + j], src.data[i * src.step + j]);
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
  else {
    unsigned nc = sensor_msgs::image_encodings::numChannels(src.encoding);

    int width = dst.getWidth();
    int size = dst.getSize();
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
        dst.bitmap[idx] = vpRGBa(src.data[i * src.step + j * nc + 0], src.data[i * src.step + j * nc + 1],
                           src.data[i * src.step + j * nc + 2]);
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
  return dst;
}
}
} // namespace visp_common
