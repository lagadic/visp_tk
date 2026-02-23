``visp_common`` package
=======================

.. contents:: Table of Contents
  :depth: 3


Introduction
============

``visp_common`` is a small interface between the ViSP library and ROS. For instance it converts between the different data types used by each library.

To date, the supported functionnality sums up to:

  * `ViSP vpImage <https://visp-doc.inria.fr/doxygen/visp-daily/classvpImage.html>`__ / `ROS sensor_msgs::Image <https://docs.ros.org/en/api/sensor_msgs/html/msg/Image.html>`__ conversion

  * `ViSP vpCameraParameters <https://visp-doc.inria.fr/doxygen/visp-daily/classvpCameraParameters.html>`__ / `ROS sensor_msgs::CameraInfo <http://www.ros.org/doc/api/sensor_msgs/html/msg/CameraInfo.html>`__ conversion

  * `ViSP vpHomogeneousMatrix <https://visp-doc.inria.fr/doxygen/visp-daily/classvpHomogeneousMatrix.html>`__ / `ROS geometry_msgs::msg::Transform <http://www.ros.org/doc/api/geometry_msgs/html/msg/Transform.html>`__ conversion

  * `ViSP vpHomogeneousMatrix <https://visp-doc.inria.fr/doxygen/visp-daily/classvpHomogeneousMatrix.html>`__ / `ROS geometry_msgs::msg::Pose <http://www.ros.org/doc/api/geometry_msgs/html/msg/Pose.html>`__ conversion

  * converting a ROS2 path ``package://pkg_name/path`` into an absolute path.

Setup
=====

This package can be compiled like any other ros2 package using ``colcon``. Choose the branch that corresponds to your ROS2 distro.

Prerequisities
--------------

Installing ViSP
+++++++++++++++

Install ViSP from ros2 package
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

First you need to install ViSP as a system dependency. This can be achived using ``ros-${ROS_DISTRO}-visp`` package available for Ubuntu. Just run:

.. code-block:: shell

	$ sudo apt-get install ros-${ROS_DISTRO}-visp


Install ViSP from source
^^^^^^^^^^^^^^^^^^^^^^^^

If the ros2 package is not available or if you want to use a more recent version of ViSP, you can also install ViSP from source following `ViSP Quick Installation <https://visp-doc.inria.fr/doxygen/visp-daily/tutorial-install-ubuntu.html#install_ubuntu_quick>`__. We recall here after the main steps:

.. code-block:: shell

    $ cd $VISP_WS
    $ git clone https://github.com/lagadic/visp.git
    $ mkdir -p $VISP_WS/visp-build
    $ cd $VISP_WS/visp-build
    $ cmake ../visp
    $ make -j$(nproc)

Then to use this version you have to setup ``VISP_DIR`` environment variable to the folder that contains the build. In our case it becomes:

.. code-block:: shell

	$ export VISP_DIR=$VISP_WS/visp-build

Installing other dependencies
+++++++++++++++++++++++++++++

You can install the other ROS2 dependencies using system installation.

.. code-block:: shell

	$ sudo apt-get install ros-${ROS_DISTRO}-ament-index-cpp \
                         ros-${ROS_DISTRO}-camera-calibration-parsers \
                         ros-${ROS_DISTRO}-geometry-msgs \
                         ros-${ROS_DISTRO}-sensor-msgs

Alternatively, you can use the ``rosdep`` utilitary, but be sure to uninstall the
``ros-${ROS_DISTRO}-visp`` package that will be installed if you want to use
ViSP compiled from source. Assuming that the ``visp_tk`` repository has been
cloned in the ``src`` directory (see `How to get and build visp_common`_), run the following commands:

.. code-block:: shell

  $ sudo rosdep init
  $ rosdep update
  $ rosdep install -i --from-path src --rosdistro ${ROS_DISTRO} -y
  $ if [ -z ${VISP_DIR+x} ]; then echo "VISP_DIR is unset, keeping ViSP system package"; else echo "VISP_DIR is set, removing system install" && sudo apt remove ros-${ROS_DISTRO}-visp; fi

How to get and build visp_common
--------------------------------

Supposed you have a ros2 work space just run:

.. code-block:: shell

    $ cd ~/colcon_ws/src
    $ git clone -b humble https://github.com/lagadic/visp_tk.git
    $ cd ..
    $ colcon build --symlink-install --packages-select visp_common


Documentation
=============

* `Project webpage on ros.org <https://docs.ros.org/en/humble/p/visp_common>`__
* `Project webpage: source code download, bug report <https://github.com/lagadic/visp_tk>`__
