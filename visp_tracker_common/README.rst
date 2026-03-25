``visp_tracker_common`` documentation
=====================================

.. contents:: Table of Contents
  :depth: 3

Introduction
============

The ``visp_tracker_common`` is a package that contains:

  - base classes for the different ViSP's tracker nodes, providing a common interface,
  - messages used by these nodes,
  - a ``tracker_gui`` node, that permits to display the tracking results on a machine different from
    the one that runs the tracker.

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

	$ sudo apt-get install ros-${ROS_DISTRO}-geometry-msgs \
                         ros-${ROS_DISTRO}-image-transport-plugins \
                         ros-${ROS_DISTRO}-message-filters \
                         ros-${ROS_DISTRO}-rclcpp \
                         ros-${ROS_DISTRO}-sensor-msgs \
                         ros-${ROS_DISTRO}-std-srvs \
                         ros-${ROS_DISTRO}-tf2 \
                         ros-${ROS_DISTRO}-tf2-ros \
                         ros-${ROS_DISTRO}-vision-msgs

Alternatively, you can use the ``rosdep`` utilitary, but be sure to uninstall the
``ros-${ROS_DISTRO}-visp`` package that will be installed if you want to use
ViSP compiled from source. Assuming that the ``visp_tk`` repository has been
cloned in the ``src`` directory (see `How to get and build visp_tracker_common`_), run the following commands:

.. code-block:: shell

  $ sudo rosdep init
  $ rosdep update
  $ rosdep install -i --from-path src --rosdistro ${ROS_DISTRO} -y
  $ if [ -z ${VISP_DIR+x} ]; then echo "VISP_DIR is unset, keeping ViSP system package"; else echo "VISP_DIR is set, removing system install" && sudo apt remove ros-${ROS_DISTRO}-visp; fi


How to get and build visp_tracker_common
----------------------------------------

Supposed you have a ros2 work space just run:

.. code-block:: shell

    $ cd ~/colcon_ws/src
    $ git clone -b humble https://github.com/lagadic/visp_tk.git
    $ cd ..
    $ colcon build --symlink-install --packages-up-to visp_tracker_common

BaseTracker node
================

This section will present the different parameters of the ``visp_tracker_common::BaseTracker`` class.

Related to subscription / publication
-------------------------------------

* **REQUIRED** ``rgb_camera_topic_name``: name of the color camera topic.
* **REQUIRED** ``rgb_stream_topic_name``: name of the color image topic.

Related to the tracking
-----------------------

* *OPTIONAL* ``config_file``: path to the configuration file to initialize the tracker. package:// will be replaced by the path to the share folder of the corresponding package.
* *OPTIONAL* ``init_method``: initialization method to initialize the tracker. Default method is initialization by click.
* *OPTIONAL* ``init_topic``: if the ``init_method`` is set to topic, this parameter becomes **REQUIRED** and must be set to the topic of type ``geometry_msgs:msg::PoseStamped`` the tracker must use to get the init pose.

Related to the display
----------------------

* *OPTIONAL* ``headless_mode``: if set to true, the node will not display anything. A TrackerGUI node will be needed in
  addition to the tracker node.
* *OPTIONAL* ``display_nb_frames_skipped``: if ``headless_mode`` is set to false, this parameter indicates the number of frames skipped during display.

BaseMultiModalTracker node
==========================

This section will present the different parameters of the ``visp_tracker_common::BaseMultiModalTracker`` class.
The node will determine if it requires depth or not depending on the tracker configuration. If the node
requires depth, the ``depth_camera_topic_name`` and ``depth_stream_topic_name`` becomes **REQUIRED**.
If the tracker requires depth, a `message_filters::Synchronizer <https://github.com/ros2/message_filters/blob/rolling/doc/index.rst>`__
is used to temporally synchronize the depth stream and the color stream. **NB**: using the ``TrackerGUI`` class
on the same computer than a class inheriting from ``visp_tracker_common::BaseMultiModalTracker`` and using depth
leads to the freezing of the tracker, due to a desynchronization of the color and depth streams.

Related to subscription / publication
-------------------------------------

* *OPTIONAL* ``depth_camera_topic_name``: name of the depth camera topic.
* *OPTIONAL* ``depth_stream_topic_name``: name of the depth stream topic.
* *OPTIONAL* ``stream_qos_durability``: the durability of both the RGB and depth image streams (they need to be the same) if depth is required.
* *OPTIONAL* ``stream_qos_reliability``: the reliability of both the RGB and depth image streams (they need to be the same) if depth is required.
* *OPTIONAL* ``stream_qos_depth``: the depth of the queue of both the RGB and depth image streams (they need to be the same) if depth is required.

Related to the tracking
-----------------------

* *OPTIONAL* ``z_factor``: factor to convert the depth image expressed as uint16_t into meters. For instance, if a value
  of ``1000`` in the raw depth image corresponds to ``1 meter``, the ``z_factor`` must be set to ``0.001``.

``tracker_gui`` node
====================

This section will present the different parameters of the ``tracker_gui`` node. This node permits to
have a Graphical User Interface (GUI) on a remote computer, i.e. on another computer than the one that is running the tracker node(s). It should not
be used when you can run the tracker on a computer that has actual display capabilities, especialy with mulimodal trackers,
because it makes out of synchronization the RGB and depth streams, freezing the tracker.

Related to subscription / publication
-------------------------------------

RGB-stream-related
^^^^^^^^^^^^^^^^^^

* *OPTIONAL* ``image_transport``: the type of image transport compression algorithm used. Default is ``compressed``. See `image_transport_plugins <https://github.com/ros-perception/image_transport_plugins>`__ for more information.
* **REQUIRED** ``camera_topic``: the name of the camera information topic of the RGB stream.
* **REQUIRED** ``color_topic``: the name of the raw RGB stream (e.g. ``/camera/color/image_raw``). The ``image_transport`` parameter will then be used to determine the sub topic that will be used.
* *OPTIONAL* ``color_qos_queue_depth``: the depth of the queue of the RGB topic.
* *OPTIONAL* ``color_qos_durability``: the durability of the messages of the RGB topic.
* *OPTIONAL* ``color_qos_reliability``: the reliability of the messages of the RGB topic.

Depth-stream-related
^^^^^^^^^^^^^^^^^^^^

* *OPTIONAL* ``use_depth``: if true, the ``TrackerGUI`` will subscribe to the depth topic.
* *OPTIONAL* ``depth_image_transport``: the type of image transport compression algorithm used for the depth stream. Default is ``compressedDepth``. See `image_transport_plugins <https://github.com/ros-perception/image_transport_plugins>`__ for more information.
* *OPTIONAL* ``depth_topic``: the name of the raw depth stream (e.g. ``/camera/depth/image_raw``). The ``image_transport`` subscriber will then subscribe to the ``compressedDepth`` sub-topic.
* *OPTIONAL* ``depth_qos_queue_depth``: the depth of the queue of the depth topic.
* *OPTIONAL* ``depth_qos_durability``: the durability of the messages of the depth topic.
* *OPTIONAL* ``depth_qos_reliability``: the reliability of the messages of the depth topic.

Features-related
^^^^^^^^^^^^^^^^

* *OPTIONAL* ``apriltag_topics``: a vector of strings. Each item corresponds to the name of a ``visp_tracker_common::msg::AprilTagDetectionArray`` topic.
* *OPTIONAL* ``features_topics``: a vector of strings. Each item corresponds to the name of a ``visp_tracker_common::msg::NamedFeatureArray`` topic.
* *OPTIONAL* ``poses_topics``: a vector of strings. Each item corresponds to the name of a ``geometry_msgs::msg::PoseStamped`` topic.

Related to services
-------------------

* **REQUIRED** ``client_nodes``: a vector of strings. Each item corresponds to the name of a node that inherits either from ``visp_tracker_common::BaseTracker`` or ``visp_tracker_common::BaseMultiModalTracker``.
  This name will be used to compose the name of services that can be called upon.

Related to display
------------------

* *OPTIONAL* ``poses_names``: a vector of strings. Each item corresponds to the name to display beside the message that comes from the topic listed ``poses_topics`` which has
  the same index than the item. **Important**: must have the same size than ``poses_topics``.
* *OPTIONAL* ``depth_scale``: the scale to transform the raw depth image into a depth image expressed in meters. (e.g. if the raw depth image is encoded such as ``1000`` corresponds to ``1m``, the parameter must be set to ``0.001``).
* *OPTIONAL* ``min_depth``: the minimum depth to display, in meters.
* *OPTIONAL* ``max_depth``: the maximum depth to display, in meters.
* *OPTIONAL* ``features_thickness``: the thickness of the features published in the ``features_topics``, in pixels.
* *OPTIONAL* ``features_type``: the type of feature to use to display 2D points, if any are present in the ``features_topics``.
