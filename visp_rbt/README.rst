``visp_rbt`` documentation
==============================

.. contents:: Table of Contents
  :depth: 3

Introduction
============

The ``visp_rbt`` furnishes a node that is a wrapper over the ``vpRBTracker`` class of `ViSP <https://visp-doc.inria.fr/doxygen/visp-daily/classvpRBTracker.html>`__.
This Render-Based Tracker (RBT) permits to track an object whose 3D model is known.

The tracker must be configured using a JSON configuration file (see `BaseTracker documentation <../visp_tracker_common/index.html#related-to-the-tracking>`__ ).
See `ViSP documentation <https://visp-doc.inria.fr/doxygen/visp-daily/tutorial-tracking-rbt.html#rbt_tracking_config_json>`__
for more information on the configuration of the ``vpRBTracker`` class using JSON.

**IMPORTANT**: When the tracker uses depth information, when the ``tracker_gui`` node was run on the same computer that the ``visp_rbt_node``,
the tracker node has been seen freezing. It is probably due to the fact that the ``tracker_gui`` made the RGB and depth
streams out of synchronization. The ``visp_rbt_node`` having display capabilities, if you want a visual feedback on the
same computer than the one you are running the ``visp_rbt_node``, please run it activating the display. (Please refer to
`BaseTracker documentation <../visp_tracker_common/index.html#related-to-the-display>`__ for more information).

**IMPORTANT**: the ``visp_rbt_node`` can be really slow when OpenMP is ill-configured. Two solutions exist:
  - run ``export GOMP_SPINCOUNT=0`` in the terminal in which you will run the ``visp_rbt_node``,
  - insert the following piece of code in your Python launch file:
  .. code-block:: Python

    from launch.actions import SetEnvironmentVariable
    def generate_launch_description():
      ld = LaunchDescription()
      ...
      ld.add_action(SetEnvironmentVariable(name='GOMP_SPINCOUNT', value='0'))

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
cloned in the ``src`` directory (see `How to get and build visp_rbt`_), run the following commands:

.. code-block:: shell

  $ sudo rosdep init
  $ rosdep update
  $ rosdep install -i --from-path src --rosdistro ${ROS_DISTRO} -y
  $ if [ -z ${VISP_DIR+x} ]; then echo "VISP_DIR is unset, keeping ViSP system package"; else echo "VISP_DIR is set, removing system install" && sudo apt remove ros-${ROS_DISTRO}-visp; fi


How to get and build visp_rbt
-----------------------------

Supposed you have a ros2 work space just run:

.. code-block:: shell

    $ cd ~/colcon_ws/src
    $ git clone -b humble https://github.com/lagadic/visp_tk.git
    $ cd ..
    $ colcon build --symlink-install --packages-up-to visp_rbt

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

Related to display
------------------

- *OPTIONAL* ``max_z_display``: the maximum depth we want to display.
