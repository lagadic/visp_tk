``visp_apriltag`` documentation
===============================

.. contents:: Table of Contents
  :depth: 3

Introduction
============

The ``visp_apriltag`` furnishes a node that is a wrapper over the ``vpDetectorAprilTag`` class of `ViSP <https://visp-doc.inria.fr/doxygen/visp-daily/classvpDetectorAprilTag.html>`__.
It permits to detect AprilTag and ArUco tags in an image.

The tracker can either be configured using a configuration file (see `BaseTracker documentation <../visp_tracker_common/index.html#related-to-the-tracking>`__ ) or through its parameters (see `Node parameters`_).

The node will publish a ``/<node_name>/tags_info``
topic that will contain information about the detected tags. See `the message definition <../visp_tracker_common/interfaces/msg/AprilTagDetection.html>`__ to see which information
are published.

Installation instructions
=========================

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

	$ sudo apt-get install ros-${ROS_DISTRO}-rclcpp \
                         ros-${ROS_DISTRO}-sensor-msgs

Alternatively, you can use the ``rosdep`` utilitary, but be sure to uninstall the
``ros-${ROS_DISTRO}-visp`` package that will be installed if you want to use
ViSP compiled from source. Assuming that the ``visp_tk`` repository has been
cloned in the ``src`` directory (see `How to get and build visp_apriltag`_), run the following commands:

.. code-block:: shell

  $ sudo rosdep init
  $ rosdep update
  $ rosdep install -i --from-path src --rosdistro ${ROS_DISTRO} -y
  $ if [ -z ${VISP_DIR+x} ]; then echo "VISP_DIR is unset, keeping ViSP system package"; else echo "VISP_DIR is set, removing system install" && sudo apt remove ros-${ROS_DISTRO}-visp; fi


How to get and build visp_apriltag
----------------------------------

Supposed you have a ros2 work space just run:

.. code-block:: shell

    $ cd ~/colcon_ws/src
    $ git clone -b humble https://github.com/lagadic/visp_tk.git
    $ cd ..
    $ colcon build --symlink-install --packages-up-to visp_apriltag

Node parameters
===============

This section will present the different parameters of the node, that exist
in addition to the ones it inherits from the ``visp_tracker_common::BaseTracker`` class.
See `visp_tracker_common`_ documentation.

.. _visp_tracker_common: ../visp_tracker_common/index.html#basetracker-node

Related to the tag detection
----------------------------

- *OPTIONAL* ``tag_family``: if ``config_file`` is not set, this parameter becomes **REQUIRED**. It corresponds to the tag family the node will have to detect. See `ViSP documentation <https://visp-doc.inria.fr/doxygen/visp-daily/classvpDetectorAprilTag.html>`__ for more information.
- *OPTIONAL* ``detection_margin_thresh``: the detection margin threshold. See `ViSP documentation <https://visp-doc.inria.fr/doxygen/visp-daily/classvpDetectorAprilTag.html>`__ for more information.

Related to the pose computation
-------------------------------

- *OPTIONAL* ``tag_size``: if ``config_file`` is not set, this parameter becomes **REQUIRED**. It corresponds to the dimension of the tag, in meters. See `ViSP documentation <https://visp-doc.inria.fr/doxygen/visp-daily/classvpDetectorAprilTag.html>`__ for more information.
- *OPTIONAL* ``pose_method``: if ``config_file`` is not set, this parameter becomes **REQUIRED**. It corresponds to the method to use to compute the pose of a tag. See `ViSP documentation <https://visp-doc.inria.fr/doxygen/visp-daily/classvpDetectorAprilTag.html>`__ for more information.
- *OPTIONAL* ``align_z``: if true, the Z-axis will be aligned with the Z-axis of the camera.
- *OPTIONAL* ``id_published``: if set, the node will publish the pose of the tag whose ID corresponds to this attribute
  in the pose topic inherited from the ``visp_tracker_common::BaseTracker`` class.

Related to display
------------------

- *OPTIONAL* ``display_tag``: if true, the tag borders will be displayed.
