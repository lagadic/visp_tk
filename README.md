ViSP stack for ROS 2
====================

[![License: GPL v2+](https://img.shields.io/badge/License-GPL%20v2%2B-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html)

## 1. Introduction

`visp_tk` contains ROS 2 packages designed to interface ROS 2 with [ViSP](https://visp.inria.fr), a library tailored for
visual-servoing and visual tracking applications. This repository includes:

- **visp_common**: Bridge between ROS 2 image and geometry messages and ViSP image and 3D transformation representations.
- **visp_tracker_common**: Common tools for tracker packages, such as graphical user interfaces (GUI).
- **visp_apriltag**: ROS 2 node wrapping the AprilTag detector of ViSP.
- **visp_mbt**: ROS 2 node wrapping the Model-Based Tracker (MBT) of ViSP.
- **visp_rbt**: ROS 2 node wrapping the Render-Based Tracker (RBT) of ViSP.

##  2. Prerequisities
### 2.1. Install ROS 2

Make sure your ROS 2 core environment is installed. Refer to the official
[ROS 2 documentation](https://docs.ros.org/) to get started.

### 2.2. Install ViSP

Please refer to the official installation instructions from the
[ViSP installation tutorials](https://visp-doc.inria.fr/doxygen/visp-daily/tutorial_install.html).

**NOTE**
- Pre-built ViSP packages exist for Ubuntu (`libvisp-dev`) and ROS 2 (`ros2-<distro>-visp`), but they are usually built
  against a reduced number of third-party libraries. Consequently, you might miss advanced features required to control
  hardware (e.g., Franka robots), acquire images from RealSense cameras, or leverage the Panda3D dependency needed for
  the `visp_rbt` package.
- That's why **we strongly recommend building ViSP from source.** See [tutorials](https://visp-doc.inria.fr/doxygen/visp-daily/tutorial_install_src.html).
- After building ViSP from source, remember to set the `VISP_DIR` environment variable to your build directory,
  for example:
  ```bash
	$ export VISP_DIR=$VISP_WS/visp-build
  ```

## 3. Build `visp_tk`

- Source your ROS 2 installation:
  ```bash
  source /opt/ros/<distro>/setup.bash
  ```

- Verify that ROS 2 is correctly detected. You should see environment variables matching your distribution
  (e.g., humble, jazzy, lyrical, rolling):

  ```bash
  env | grep ROS
  ```
  Expected output example:
  ```
  ROS_VERSION=2
  ROS_PYTHON_VERSION=3
  ROS_DISTRO=jazzy
  ```

- Clone the repository into your ROS 2 workspace source directory, checking out the branch corresponding to your
  ROS distribution:

  ```bash
  cd <YOUR_ROS2_WORKSPACE>/src
  git clone https://github.com/lagadic/visp_tk.git -b $ROS_DISTRO
  ```

- Install required ROS 2 dependencies via rosdep:

  ```bash
  cd <YOUR_ROS2_WORKSPACE>
  rosdep update && rosdep install --from-paths src --ignore-src
  ```

- Build the `visp_tk` packages using `colcon`:

  ```bash
  colcon build --symlink-install --packages-up-to visp_tk
  ```

  If ViSP is not found, use `VISP_DIR` to point to `$VISP_WS/visp-build` folder like:

  ```bash
  colcon build --symlink-install --packages-up-to visp_tk --cmake-args -DVISP_DIR=$VISP_WS/visp-build
  ```

- Alternatively, build the `visp_tk_tutorials` package to see examples of use::

  ```bash
  colcon build --symlink-install --packages-up-to visp_tk_tutorials
  ```

## 4. Build documentation

- To generate the package documentation, first install `rosdoc2` if you haven't already:
  ```bash
  sudo apt update && sudo apt install python3-rosdoc2
  ```
- Next, build the documentation for each package by running:
  ```bash
  for pkg in visp_apriltag visp_common visp_mbt visp_rbt visp_tk_tutorials visp_tracker_common; do
    rosdoc2 build --package-path src/visp_tk/$pkg
  done
  ```
- **Fix build conflicts with generated docs:** Ignore documentation directories in `colcon` by creating empty
  `COLCON_IGNORE` files:
  ```bash
  touch docs_build/COLCON_IGNORE cross_reference/COLCON_IGNORE docs_output/COLCON_IGNORE
  ```

- The generated documentation will be available in the `docs_output` folder.

## 5. Usage

- Once the workspace is successfully built, don't forget to source your local setup:

  ```bash
  source install/setup.bash
  ```

- Running tutorials

  - Apriltag detection using a rosbag

    ```bash
    ros2 launch visp_tk_tutorials apriltag_tracker_bag_launch.py
    ```

  - Apriltag detection using a camera compatible with video4Linux (typically a webcam)

    ```bash
    ros2 launch visp_tk_tutorials apriltag_tracker_live_v4l_launch.py
    ```

  - Model-Based Tracker using a rosbag

    ```bash
    ros2 launch visp_tk_tutorials mbt_json_launch.py
    ```

- Running individual nodes

  - AprilTag Detector Node:

    ```bash
    ros2 run visp_apriltag visp_apriltag_node --ros-args -p size:=0.05
    ```

  - Model-Based Tracker (MBT) Node:
    ```bash
    ros2 run visp_mbt visp_mbt_node
    ```

  Detailed launch files, parameters description, and configuration examples for each package are available in their respective subdirectories.

## 6. Support & Contributing

- ViSP Website: https://visp.inria.fr

- Bug Tracker & Issues: [GitHub Issues](https://github.com/lagadic/visp_tk/issues)
