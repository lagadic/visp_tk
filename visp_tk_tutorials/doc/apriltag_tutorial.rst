AprilTag tutorial
=================

This is a tutorial for the `visp_apriltag` package. The documentation of the AprilTag and ArUco tags detector provided
by ViSP can be found `here <https://visp-doc.inria.fr/doxygen/visp-daily/classvpDetectorAprilTag.html>`__
ViSP documentation also provides step-by-step tutorials to progressively learn how to use the detector
`here <https://visp-doc.inria.fr/doxygen/visp-daily/tutorial-detection-apriltag.htmll>`__ .

Consequently, this tutorial will not focus on explaining the meaning of the different parameters of the
detector, but instead focus on the specificities on how to use the ``visp_apriltag`` package.

We provide two examples on how to use the `visp_apriltag` package: one that works with a live camera thanks to
the `v4l2_camera` package, and one that uses a pre-recorded rosbag. We will first have a look to the tutorial
that uses a live camera.

Tutorial using a live camera
----------------------------

Purpose of this tutorial
++++++++++++++++++++++++

The purpose of this tutorial is to have a simple situation where we assume that all the tags have the same dimension.

How to launch it
++++++++++++++++
If you compiled the packages, you have to source the workspace in which you have compiled the packages.
Otherwise, if you installed the packages using the package manager, you only have to source the `/opt/ros/${ROS_DISTRO}/setup.bash`
file (or one of the other `setup` files if you are not using bash).

Then you can run:

.. code-block:: shell

  ros2 launch visp_tk_tutorials apriltag_tracker_live_launch.py

If you get the following error when launching the launch file, it is because your camera has not been calibrated yet.
See for instance the documentation of the `camera_calibration package <https://docs.ros.org/en/rolling/p/camera_calibration/doc/tutorial_mono.html>`__
to see how to calibrate your camera.

.. code-block:: shell

  [camera.v4l2_camera]: using default calibration URL
  [v4l2_camera_node-1] [INFO]  [camera.v4l2_camera]: camera calibration URL: file:///home/${USER}/.ros/camera_info/integrated_webcam_hd:_integrate.yaml
  [v4l2_camera_node-1] [ERROR] [camera_calibration_parsers]: Unable to open camera calibration file [/home/${USER}/.ros/camera_info/integrated_webcam_hd:_integrate.yaml]
  [v4l2_camera_node-1] [WARN]  [camera.v4l2_camera]: Camera calibration file /home/${USER}/.ros/camera_info/integrated_webcam_hd:_integrate.yaml not found
  [visp_apriltag_node-2] [INFO] [tracker_apriltag]: Receive image
  [visp_apriltag_node-2] [INFO] [tracker_apriltag]: Waiting for camera info...
  [visp_apriltag_node-2] terminate called after throwing an instance of 'std::runtime_error'
  [visp_apriltag_node-2]   what():  uncalibrated camera

The launch file accepts different arguments. To have the list and the explanations related to the
arguments, please run:

.. code-block:: shell

  ros2 launch visp_tk_tutorials apriltag_tracker_live_launch.py --show-args

Code explanation
++++++++++++++++

Let's have a look at ``apriltag_tracker_live_launch.py``:
.. literalinclude:: /_code/launch/apriltag_tracker_live_launch.py
  :language: python
  :linenos:

We will break it down to better understand it.

First, we begin by declaring the parameters that will be needed by the `v4l2_camera` node. See the
`v4l2_camera documentation <https://gitlab.com/boldhearts/ros2_v4l2_camera/#nodes>`__ for
more details on these parameters.

.. literalinclude:: /_code/launch/apriltag_tracker_live_launch.py
  :language: python
  :lineno-match:
  :start-after: BEGIN_V4L2_ARGUMENTS
  :end-before: END_V4L2_ARGUMENTS

Then, we declare the parameters of the `visp_apriltag` node. See
`visp_apriltag documentation <../visp_apriltag/index.html#node-parameters>`__
for a detailed explanation of its parameters.

.. literalinclude:: /_code/launch/apriltag_tracker_live_launch.py
  :language: python
  :lineno-match:
  :start-after: BEGIN_APRILTAG_ARGUMENTS
  :end-before: END_APRILTAG_ARGUMENTS

The important thing to notice is that the behavior of the `visp_apriltag` node if the launch file is launched
without any arguments is to consider that all the tags have a size of `0.1` meter and that the node will not publish
any messages on the `pose` topic.

Then, we create the `v4l2_camera` node, using some launch arguments and some hard-coded values for its parameters:

.. literalinclude:: /_code/launch/apriltag_tracker_live_launch.py
  :language: python
  :lineno-match:
  :start-after: BEGIN_V4L2_NODE
  :end-before: END_V4L2_NODE

Then, we create the `visp_apriltag` node, using some launch arguments and some hard-coded values for its parameters:

.. literalinclude:: /_code/launch/apriltag_tracker_live_launch.py
  :language: python
  :lineno-match:
  :start-after: BEGIN_APRILTAG_NODE
  :end-before: END_APRILTAG_NODE

Finally, we ask to stop all the nodes once the `visp_apriltag` is killed by a right click on the window.

.. literalinclude:: /_code/launch/apriltag_tracker_live_launch.py
  :language: python
  :lineno-match:
  :start-after: BEGIN_SHUTDOWN
  :end-before: END_SHUTDOWN

Tutorial using a rosbag
-----------------------

Purpose of this tutorial
++++++++++++++++++++++++

The purpose of this tutorial is to learn how to handle situations where the tags may have different sizes.

How to launch it
++++++++++++++++

If you compiled the packages, you have to source the workspace in which you have compiled the packages.
Otherwise, if you installed the packages using the package manager, you only have to source the `/opt/ros/${ROS_DISTRO}/setup.bash`
file (or one of the other `setup` files if you are not using bash).

Then you can run:

.. code-block:: shell

  ros2 launch visp_tk_tutorials apriltag_tracker_bag_launch.py

When you first start the launch file, you should see something similar to the following image:

.. figure:: images/apriltag_bag_start.png
   :width: 90%
   :align: center

After left clicking on the image, the detector will be turned on, and you should see the result of
the detection on screen, which sould look like the following image:

.. figure:: images/apriltag_bag_expected_output.png
   :width: 90%
   :align: center


Code explanation
++++++++++++++++

Let's have a look at ``apriltag_tracker_bag_launch.py``:

First, we declare the parameters of the `visp_apriltag` node. See
`visp_apriltag documentation <../visp_apriltag/index.html#node-parameters>`__
for a detailed explanation of its parameters.

.. literalinclude:: /_code/launch/apriltag_tracker_bag_launch.py
  :language: python
  :lineno-match:
  :start-after: BEGIN_APRILTAG_ARGUMENTS
  :end-before: END_APRILTAG_ARGUMENTS

The important thing to notice is that the behavior of the `visp_apriltag` node if the launch file is launched
without any arguments is to consider that all the tags whose ID is `0` have a size of `0.2315` meter,
all the tags whose ID is `1` have a size of `0.064` meter and all the other tags have a size of `0.2315` meter.
Additionally, that the node will not publish any messages on the `pose` topic.

Then, we create the process that will read the rosbag in a loop manner:

.. literalinclude:: /_code/launch/apriltag_tracker_bag_launch.py
  :language: python
  :lineno-match:
  :start-after: BEGIN_ROSBAG_PLAYER
  :end-before: END_ROSBAG_PLAYER

Then, we create the `visp_apriltag` node, using the values of the launch arguments:

.. literalinclude:: /_code/launch/apriltag_tracker_bag_launch.py
  :language: python
  :lineno-match:
  :start-after: BEGIN_APRILTAG_NODE
  :end-before: END_APRILTAG_NODE

Finally, we ask to stop all the nodes once the `visp_apriltag` is killed by a right click on the window.

.. literalinclude:: /_code/launch/apriltag_tracker_bag_launch.py
  :language: python
  :lineno-match:
  :start-after: BEGIN_SHUTDOWN
  :end-before: END_SHUTDOWN
