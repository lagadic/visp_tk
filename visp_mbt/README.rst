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


How to get and build visp_mbt
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

  * Build the ``visp_mbt`` package:

    .. code-block:: shell

      colcon build --symlink-install --packages-up-to visp_mbt

    .. Note::

      If you encounter the following issue:

        .. code-block:: shell

          --- stderr: visp_mbt
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

          colcon build --symlink-install --packages-up-to visp_mbt --cmake-args -DVISP_DIR=$VISP_WS/visp-build




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
