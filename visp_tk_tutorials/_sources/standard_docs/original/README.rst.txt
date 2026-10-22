``visp_tk_tutorials`` documentation
===================================

This package contains tutorials for the different packages of the `visp_tk` metapackage.

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
    dependency needed for the `visp_tk_tutorials` package.
  * That's why **we strongly recommend building ViSP from source.**
    See `tutorials <https://visp-doc.inria.fr/doxygen/visp-daily/tutorial_install_src.html>`__.
  * After building ViSP from source, remember to set the `VISP_DIR` environment variable to your build directory,
    for example:

    .. code-block:: shell

      export VISP_DIR=$VISP_WS/visp-build


How to get and build visp_tk_tutorials
======================================

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

  * Build the ``visp_tk_tutorials`` package:

    .. code-block:: shell

      colcon build --symlink-install --packages-up-to visp_tk_tutorials

    .. Note::

      If you encounter the following issue:

        .. code-block:: shell

          --- stderr: visp_tk_tutorials
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

          colcon build --symlink-install --packages-up-to visp_tk_tutorials --cmake-args -DVISP_DIR=$VISP_WS/visp-build
