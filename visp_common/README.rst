``visp_common`` package
=======================

``visp_common`` is a small interface between the ViSP library and ROS. For instance it converts between the different data types used by each library.

To date, the supported functionnality sums up to:
 * `ViSP vpImage <https://visp-doc.inria.fr/doxygen/visp-daily/classvpImage.html>`__ / `ROS sensor_msgs::Image <https://docs.ros.org/en/api/sensor_msgs/html/msg/Image.html>`__ conversion
 * `ViSP vpCameraParameters <https://visp-doc.inria.fr/doxygen/visp-daily/classvpCameraParameters.html>`__ / `ROS sensor_msgs::CameraInfo <http://www.ros.org/doc/api/sensor_msgs/html/msg/CameraInfo.html>`__ conversion
 * `ViSP vpHomogeneousMatrix <https://visp-doc.inria.fr/doxygen/visp-daily/classvpHomogeneousMatrix.html>`__ / `ROS geometry_msgs::msg::Transform <http://www.ros.org/doc/api/geometry_msgs/html/msg/Transform.html>`__ conversion
 * `ViSP vpHomogeneousMatrix <https://visp-doc.inria.fr/doxygen/visp-daily/classvpHomogeneousMatrix.html>`__ / `ROS geometry_msgs::msg::Pose <http://www.ros.org/doc/api/geometry_msgs/html/msg/Pose.html>`__ conversion

=====
Setup
=====

This package can be compiled like any other ros2 package using ``colcon``. Choose the branch that corresponds to your ROS2 distro.

--------------
Prerequisities
--------------

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Install ViSP from ros2 package
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

First you need to install ViSP as a system dependency. This can be achived using ``ros-${ROS-DISTRO}-visp`` package available for Ubuntu. Just run::

	$ sudo apt-get install ros-${ROS-DISTRO}-visp

^^^^^^^^^^^^^^^^^^^^^^^^
Install ViSP from source
^^^^^^^^^^^^^^^^^^^^^^^^

If the ros2 package is not available or if you want to use a more recent version of ViSP, you can also install ViSP from source following `ViSP Quick Installation <https://visp-doc.inria.fr/doxygen/visp-daily/tutorial-install-ubuntu.html#install_ubuntu_quick>`__. We recall here after the main steps::

    $ cd $VISP_WS
    $ git clone https://github.com/lagadic/visp.git
    $ mkdir -p $VISP_WS/visp-build
    $ cd $VISP_WS/visp-build
    $ cmake ../visp
    $ make -j$(nproc)

Then to use this version you have to setup ``VISP_DIR`` environment variable to the folder that contains the build. In our case it becomes::

	$ export VISP_DIR=$VISP_WS/visp-build

--------------------------------
How to get and build visp_common
--------------------------------

Supposed you have a ros2 work space just run::

  $ cd ~/colcon_ws/src
	$ git clone -b humble https://github.com/lagadic/visp_tk.git
	$ cd ..
	$ colcon build --symlink-install --packages-select visp_common

=============
Documentation
=============

* `Project webpage on ros.org <https://docs.ros.org/en/humble/p/visp_common>`__
* `Project webpage: source code download, bug report <https://github.com/lagadic/visp_tk>`__
