ViSP stack for ROS
==================

![GPL-2](https://www.gnu.org/graphics/gplv3-127x51.png)

## 1. Introduction

ROS 2 visp_tk contains packages to interface ROS 2 with [ViSP](https://visp.inria.fr) which is a library designed
for visual-servoing and visual tracking applications. This repository contains:

- visp_common: Bridge between ROS 2 image and geometry messages and ViSP image and 3D transformation representation.
- visp_tracker_common: Common tools for the tracker packages, such as a GUI
- visp_apriltag: a package that contains a ROS2 based on the AprilTag detector of ViSP.
- visp_mbt: a package that contains a ROS2 node based on the Model-Based Tracker (MBT) of ViSP
- visp_rbt: a package that contains a ROS2 node based on the Render-Based Tracker (RBT) of ViSP

##  2. Install dependencies
### 2.1. Install ROS 2

Firstly, it assumes that the ROS 2 core has already been installed, please refer to
[ROS 2 installation](https://docs.ros.org/en/rolling/Installation.html) to get started.

### 2.2. Install ViSP

Please refer to the official installation guide from [ViSP installation tutorials](https://visp-doc.inria.fr/doxygen/visp-daily/tutorial_install.html).

## 3. Build vision_visp

Fetch the latest code and build

```
$ cd <YOUR_ROS2_WORKSPACE>/src
$ git clone https://github.com/lagadic/visp_tk.git -b humble
$ cd ..
$ colcon build --symlink-install --packages-up-to visp_tk
```

If ViSP is not found, use `VISP_DIR` to point to `$VISP_WS/visp-build` folder like:

```
$ colcon build --symlink-install --cmake-args -DVISP_DIR=$VISP_WS/visp-build --packages-up-to visp_tk
```

## 4. Usage

TODO
