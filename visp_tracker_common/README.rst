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

* ``name``: info

BaseMultiModalTracker node
==========================

This section will present the different parameters of the ``visp_tracker_common::BaseMultiModalTracker`` class.

Related to subscription / publication
-------------------------------------

* ``name``: info

TrackerGUI node
===============

This section will present the different parameters of the ``visp_tracker_common::TrackerGUI`` node. This node permits to
have a GUI on a remote computer, i.e. on another computer than the one that is running the tracker node(s). It is not
useful when you can run the tracker on a computer that has actual display capabilities.

Related to subscription / publication
-------------------------------------

RGB-stream-related
^^^^^^^^^^^^^^^^^^

* *OPTIONAL* ``image_transport``: the type of image transport compression algorithm used. See `image_transport_plugins <https://github.com/ros-perception/image_transport_plugins>`__ for more information.
* **REQUIRED** ``camera_topic``: the name of the camera information topic of the RGB stream.
* **REQUIRED** ``color_topic``: the name of the raw RGB stream (e.g. ``/camera/color/image_raw``). The ``image_transport`` parameter will then be used to determine the sub topic that will be used.
* *OPTIONAL* ``color_qos_queue_depth``: the depth of the queue of the RGB topic.
* *OPTIONAL* ``color_qos_durability``: the durability of the messages of the RGB topic.
* *OPTIONAL* ``color_qos_reliability``: the reliability of the messages of the RGB topic.

Depth-stream-related
^^^^^^^^^^^^^^^^^^^^

* *OPTIONAL* ``use_depth``: if true, the ``TrackerGUI`` will subscribe to the depth topic.
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
