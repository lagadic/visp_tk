``visp_tracker_common`` documentation
=====================================

.. contents:: Table of Contents
  :depth: 3

Introduction
============

Installation instructions
=========================

BLABLA

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
* *OPTIONAL* ``display_nb_frames_skipped``: if ``headless_mode`` is set to false,

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

TrackerGUI node
===============

This section will present the different parameters of the ``visp_tracker_common::TrackerGUI`` node. This node permits to
have a GUI on a remote computer, i.e. on another computer than the one that is running the tracker node(s). It should not
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
