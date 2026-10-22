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

Prerequisities
==============

Install ROS 2
-------------

Make sure your ROS 2 core environment is installed. Refer to the official
`ROS 2 documentation <https://docs.ros.org/>`__ to get started.

Install ViSP
------------

Please refer to the official installation instructions from the
`ViSP installation tutorials <https://visp-doc.inria.fr/doxygen/visp-daily/tutorial_install.html>`__.

.. Note::

  * Pre-built ViSP packages exist for Ubuntu (`libvisp-dev`) and ROS 2 (`ros2-<distro>-visp`), but they are usually
    built against a reduced number of third-party libraries. Consequently, you might miss advanced features required
    to control hardware (e.g., Franka robots), acquire images from RealSense cameras, or leverage the Panda3D
    dependency needed for the `visp_rbt` package.
  * That's why **we strongly recommend building ViSP from source.**
    See `tutorials <https://visp-doc.inria.fr/doxygen/visp-daily/tutorial_install_src.html>`__.
  * After building ViSP from source, remember to set the `VISP_DIR` environment variable to your build directory,
    for example:

    .. code-block:: shell

      export VISP_DIR=$VISP_WS/visp-build


How to get and build visp_rbt
=============================

Here we suppose that you have a ROS 2  workspace in ``~/colcon_ws/`` folder.

  * Clone the repository into your workspace source directory, checking out the branch corresponding to your
    ROS distribution:

    .. code-block:: shell

      cd ~/colcon_ws/src
      git clone -b $ROS_DISTRO https://github.com/lagadic/visp_tk.git
      cd ..

  * Install required ROS 2 dependencies via ``rosdep``:

    .. code-block:: shell

      rosdep update && rosdep install --from-paths src --ignore-src

  * Build the ``visp_rbt`` package:

    .. code-block:: shell

      colcon build --symlink-install --packages-up-to visp_rbt

    .. Note::

      If you encounter the following issue:

        .. code-block:: shell

          --- stderr: visp_rbt
          CMake Error at CMakeLists.txt:46 (find_package):
            By not providing "FindVISP.cmake" in CMAKE_MODULE_PATH this project has
            asked CMake to find a package configuration file provided by "VISP", but
            CMake did not find one.

            Could not find a package configuration file provided by "VISP" (requested
            version 3.7) with any of the following names:

              VISPConfig.cmake
              visp-config.cmake

            Add the installation prefix of "VISP" to CMAKE_PREFIX_PATH or set
            "VISP_DIR" to a directory containing one of the above files.  If "VISP"
            provides a separate development package or SDK, be sure it has been
            installed.

      it means that ViSP is not found. Use ``VISP_DIR`` to point to ``$VISP_WS/visp-build`` folder like:

        .. code-block:: shell

          colcon build --symlink-install --packages-up-to visp_rbt --cmake-args -DVISP_DIR=$VISP_WS/visp-build


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

- *OPTIONAL* ``"package://visp_tk_tutorials/config/rbt/models/dragon/dragon.obj"``: if the JSON configuration file
does not contain the keyword ``model``, this parameter becomes **REQUIRED**. In this case, this parameter must be set to
the path towards the model file of the object to track. Note that if the keyword ``model`` is set in the JSON
configuration file, this parameter is **ignored**.

Related to display
------------------

- *OPTIONAL* ``max_z_display``: the maximum depth we want to display.
