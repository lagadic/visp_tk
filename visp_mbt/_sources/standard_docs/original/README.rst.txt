``visp_mbt`` documentation
==============================

.. contents:: Table of Contents
  :depth: 3

Introduction
============

The ``visp_mbt`` furnishes a node that is a wrapper over the ``vpMbGenericTracker`` class of `ViSP <https://visp-doc.inria.fr/doxygen/visp-daily/classvpMbGenericTracker.html>`__.
This Model-Based Tracker (MBT) permits to track an object whose 3D model is known.

The tracker must be configured using a configuration file (see `BaseTracker documentation <../visp_tracker_common/index.html#related-to-the-tracking>`__ ).
XML and JSON configuration files are handled, as long as the correct dependencies are installed. See `ViSP documentation <https://visp-doc.inria.fr/doxygen/visp-daily/classvpMbGenericTracker.html>`__
for more information.

**IMPORTANT**: When the tracker uses depth information, when the ``tracker_gui`` node was run on the same computer that the ``visp_mbt_node``,
the tracker node has been seen freezing. It is probably due to the fact that the ``tracker_gui`` made the RGB and depth
streams out of synchronization. The ``visp_mbt_node`` having display capabilities, if you want a visual feedback on the
same computer than the one you are running the ``visp_mbt_node``, please run it activating the display. (Please refer to
`BaseTracker documentation <../visp_tracker_common/index.html#related-to-the-display>`__ for more information).

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
                         ros-${ROS_DISTRO}-geometry-msgs \
                         ros-${ROS_DISTRO}-message-filters \
                         ros-${ROS_DISTRO}-sensor-msgs \
                         ros-${ROS_DISTRO}-tf2 \
                         ros-${ROS_DISTRO}-tf2-ros

Alternatively, you can use the ``rosdep`` utilitary, but be sure to uninstall the
``ros-${ROS_DISTRO}-visp`` package that will be installed if you want to use
ViSP compiled from source. Assuming that the ``visp_tk`` repository has been
cloned in the ``src`` directory (see `How to get and build visp_mbt`_), run the following commands:

.. code-block:: shell

  $ sudo rosdep init
  $ rosdep update
  $ rosdep install -i --from-path src --rosdistro ${ROS_DISTRO} -y
  $ if [ -z ${VISP_DIR+x} ]; then echo "VISP_DIR is unset, keeping ViSP system package"; else echo "VISP_DIR is set, removing system install" && sudo apt remove ros-${ROS_DISTRO}-visp; fi


How to get and build visp_mbt
-----------------------------

Supposed you have a ros2 work space just run:

.. code-block:: shell

    $ cd ~/colcon_ws/src
    $ git clone -b humble https://github.com/lagadic/visp_tk.git
    $ cd ..
    $ colcon build --symlink-install --packages-up-to visp_mbt

Node parameters
===============

This section will present the different parameters of the node, that exist
in addition to the ones it inherits from the ``visp_tracker_common::BaseMultiModalTracker`` class.
See `visp_tracker_common`_ documentation.

.. _visp_tracker_common: ../visp_tracker_common/index.html#basemultimodaltracker-node

Related to configuration files
------------------------------

- *OPIONAL* ``init_file``: if the initialization method is set to initialization by click, this parameter becomes **REQUIRED**.
  See `the documentation of BaseTracker <../visp_tracker_common/index.html#basetracker-node>`__ for more information.
  Path towards the file that contains the init points. ``package://`` will be replaced by
  the path to the share folder of the corresponding package.
- *OPTIONAL* ``rgb_model_file``: when using an XML file or not configuring the model for all trackers using a JSON file,
  this parameter becomes **REQUIRED** and must be set to the path towards the model file for the RGB tracker.
  ``package://`` will be replaced by the path to the share folder of the corresponding package.
- *OPTIONAL* ``depth_config_file``: when using an XML file, path to the configuration file to initialize the depth
  tracker, if any. ``package://`` will be replaced by the path to the share folder of the corresponding package.
- *OPTIONAL* ``depth_model_file``: when using an XML file or not configuring the model for all trackers using a JSON
  file, this parameter becomes **REQUIRED** and must be set to the path towards the model file for the depth tracker,
  if there is one. ``package://`` will be replaced by the path to the share folder of the corresponding package.

Related to configuration files
------------------------------

- *OPTIONAL* ``tracker_types``: when using an XML file, this parameter becomes **REQUIRED**. It consists in an array of
  types of trackers to use and must be of the same size than the parameter ``tracker_names``. If a tracker must have
  several types (e.g. edge tracker + klt), the types name must be separated by a ``+`` (e.g. ``edge+klt`` is a valid
  value). RGB types cannot be mixed with depth types (e.g. ``edge+depthDense`` is not valid), they must be separated.
- *OPTIONAL* ``tracker_names``: when using an XML file, this parameter becomes **REQUIRED**. It consists in an array of
  names for the different trackers (RGB and potentially depth) to use and must be of the same size than the parameter
  ``tracker_types``.
- *OPTIONAL* ``reference_tracker``: when set, the extrinsics will be loaded from a TF2 topic and this parameter must be
  a vector of size 2 such as ["``${REF_TRACKER_NAME}``","``${REF_TRACKER_FRAME_NAME}``"]. The parameter
  ``other_tracker`` becomes **REQUIRED**.
- *OPTIONAL* ``other_tracker``: when set, the extrinsics will be loaded from a TF2 topic and this parameter must be
  a vector of size 2 such as ["``${OTHER_TRACKER_NAME}``","``${OTHER_TRACKER_FRAME_NAME}``"]. The parameter
  ``reference_tracker`` becomes **REQUIRED**.

Related to automatic failure detection
--------------------------------------

- *OPTIONAL* ``detect_failure``: permits to activate the detection of tracking failure based on the projection error.
- *OPTIONAL* ``projection_error_threshold``: maximum tolerated projection error, if ``detect_failure`` is set to true.

Related to display
------------------

- *OPTIONAL* ``max_z_display``: the maximum depth we want to display.
