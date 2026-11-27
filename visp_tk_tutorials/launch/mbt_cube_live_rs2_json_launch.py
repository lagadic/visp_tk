"""
Track the textured cube with the MBT and a RealSense camera, the RealSense driver and the tracker being loaded as
components in the same (single-threaded) container with intra-process communication enabled.

cube.json combines a color tracker (moving edges and KLT keypoints, tracked on the drawings of the faces) and a
depth tracker (planes of the faces fitted to the point cloud), as teabox.json does for the teabox.
"""
import os

import yaml
from ament_index_python.packages import get_package_share_directory, PackageNotFoundError
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, RegisterEventHandler, SetEnvironmentVariable, EmitEvent, LogInfo, OpaqueFunction
from launch.events import Shutdown
from launch.event_handlers import (
    OnProcessExit
)
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import ComposableNodeContainer
from launch_ros.descriptions import ComposableNode
from launch_ros.substitutions import FindPackageShare
from launch_ros.parameter_descriptions import ParameterValue


def check_files_exist(context, *arg_names):
    """
    Check that the files given by the launch arguments arg_names exist, before anything is started.

    In a component container, a tracker that fails to load its files is only reported as a component that could not
    be loaded: the container and the camera keep running, nothing is displayed and the launch does not end.
    ``package://<package>/<path>`` is resolved to the share directory of the package, as done by the trackers.
    """
    for name in arg_names:
        path = LaunchConfiguration(name).perform(context)
        if not path:
            continue
        resolved = path
        if path.startswith("package://"):
            package, _, relative = path[len("package://"):].partition("/")
            try:
                resolved = os.path.join(get_package_share_directory(package), relative)
            except PackageNotFoundError:
                raise RuntimeError(f"Launch argument '{name}': package '{package}' of '{path}' not found")
        if not os.path.isfile(resolved):
            raise RuntimeError(f"Launch argument '{name}': file '{resolved}' does not exist")
    return []


def prepare_parameters(context):
    # Fail early, with an explicit message, if a file given as launch argument does not exist
    check_files_exist(context, "config_file", "init_file", "rgb_model_file", "depth_model_file")

    ## [Getting the launch arguments values]
    config_file = LaunchConfiguration("config_file")
    depth_camera_info_topic_name = LaunchConfiguration("depth_camera_info_topic_name")
    depth_config_file = LaunchConfiguration("depth_config_file")
    depth_image_topic_name = LaunchConfiguration("depth_image_topic_name")
    depth_model_file = LaunchConfiguration("depth_model_file")
    detect_failure = LaunchConfiguration("detect_failure")
    display_nb_frames_skipped = LaunchConfiguration("display_nb_frames_skipped")
    headless_mode = LaunchConfiguration("headless_mode")
    init_file = LaunchConfiguration("init_file")
    init_method = LaunchConfiguration("init_method")
    init_topic  = LaunchConfiguration("init_topic")
    projection_error_threshold = LaunchConfiguration("projection_error_threshold")
    rgb_camera_info_topic_name = LaunchConfiguration("rgb_camera_info_topic_name")
    rgb_model_file = LaunchConfiguration("rgb_model_file")
    rgb_image_topic_name = LaunchConfiguration("rgb_image_topic_name")
    stream_qos_depth = LaunchConfiguration("stream_qos_depth")
    stream_qos_durability = LaunchConfiguration("stream_qos_durability")
    stream_qos_reliability = LaunchConfiguration("stream_qos_reliability")
    tracker_names = LaunchConfiguration("tracker_names")
    tracker_types = LaunchConfiguration("tracker_types")
    z_factor = LaunchConfiguration("z_factor")

    parameters=[
                {'config_file' : config_file},
                {'depth_camera_info_topic_name': depth_camera_info_topic_name},
                {'depth_config_file': depth_config_file},
                {'depth_image_topic_name': depth_image_topic_name},
                {'depth_model_file': depth_model_file},
                {'detect_failure': detect_failure},
                {'display_nb_frames_skipped' : display_nb_frames_skipped},
                {'headless_mode' : headless_mode},
                {'init_file' : init_file},
                {'init_method': init_method},
                {'init_topic': init_topic},
                {'projection_error_threshold': projection_error_threshold},
                {'rgb_camera_info_topic_name': rgb_camera_info_topic_name},
                {'rgb_model_file': rgb_model_file},
                {'rgb_image_topic_name': rgb_image_topic_name},
                {'stream_qos_depth': stream_qos_depth},
                {'stream_qos_durability': stream_qos_durability},
                {'stream_qos_reliability': stream_qos_reliability},
                {'tracker_names': tracker_names},
                {'tracker_types': tracker_types},
                {'z_factor': z_factor},
                {'frame_size_ratio': ParameterValue(LaunchConfiguration('frame_size_ratio'), value_type=float)},
                {'frame_thickness': ParameterValue(LaunchConfiguration('frame_thickness'), value_type=int)}
                ]

    ## [Checking the validity of some launch arguments]
    # Trick to manage other_tracker and reference_tracker that can be empty list of strings
    other_tracker_raw = LaunchConfiguration("other_tracker").perform(context)
    reference_tracker_raw = LaunchConfiguration("reference_tracker").perform(context)
    # Forcing yaml interpretation
    try:
        other_tracker_parsed = yaml.safe_load(other_tracker_raw)
    except Exception as e:
        raise RuntimeError(f"Invalid YAML for 'other_tracker': {other_tracker_raw}") from e
    # Forcing to empty list if parsed value is None
    if other_tracker_parsed is None:
        other_tracker_parsed = []
    # Checking that the result is a list
    if not isinstance(other_tracker_parsed, list):
        raise RuntimeError(
            f"'other_tracker' must be a list, got {type(other_tracker_parsed)}"
        )
    # Adding to node parameters only if the list is non empty (empty lists are not handled by ROS2)
    if len(other_tracker_parsed)  > 0:
        parameters.append({'other_tracker': other_tracker_parsed})

    try:
        reference_tracker_parsed = yaml.safe_load(reference_tracker_raw)
    except Exception as e:
        raise RuntimeError(f"Invalid YAML for 'reference_tracker': {reference_tracker_raw}") from e

    if reference_tracker_parsed is None:
        reference_tracker_parsed = []

    if not isinstance(reference_tracker_parsed, list):
        raise RuntimeError(
            f"'reference_tracker' must be a list, got {type(reference_tracker_parsed)}"
        )

    if len(reference_tracker_parsed ) > 0:
        parameters.append({'reference_tracker': reference_tracker_parsed})
    ## [MBT tracker component]
    mbt_node = ComposableNode(
        package="visp_mbt",
        plugin="visp_mbt::MBTTracker",
        name="mbt_node",
        parameters=parameters,
    )
    ## [Realsense camera component]
    # Same parameters as when using realsense2_camera/launch/rs_launch.py, topics are published under /camera/camera
    realsense = ComposableNode(
        package="realsense2_camera",
        plugin="realsense2_camera::RealSenseNodeFactory",
        namespace="camera",
        name="camera",
        parameters=[{
            "enable_rgbd": False,
            "enable_sync": True,
            "enable_color": True,
            "enable_depth": True,
            "enable_accel": False,
            "enable_gyro": False,
            "align_depth.enable": False,
            "publish_tf": True, # publish_tf permits to have the extrinsincs expressed as a TF2
            "rgb_camera.color_profile": "640,480,30",
            "depth_module.depth_profile": "640,480,30",
        }],
        # The images are passed to the tracker without going through DDS
        extra_arguments=[{"use_intra_process_comms": True}],
    )
    ## [Component container]
    # Both components are loaded in the same process, so that the images (640x480 RGB, about 1 MB each, and depth)
    # do not go through DDS, which drops many of these large messages. The container must be single-threaded:
    # the ViSP displays used by the tracker must always be used from the thread that created them.
    container = ComposableNodeContainer(
        name="mbt_container",
        namespace="",
        package="rclcpp_components",
        executable="component_container",
        composable_node_descriptions=[realsense, mbt_node],
        output="screen",
        emulate_tty=True,
    )
    ## [Handling shutdown]
    shutdown_handler = RegisterEventHandler(
            OnProcessExit(
                target_action=container,
                on_exit=[
                    LogInfo(msg=("The container was closed, turning off the launch file")),
                    EmitEvent(event=Shutdown(
                        reason="container closed"))
                ]
            )
        )

    return [container, shutdown_handler]

def generate_launch_description():
    ld = LaunchDescription([
        DeclareLaunchArgument(
            'config_file',
            description="Absolute path towards the config file for the MBT",
            default_value=PathJoinSubstitution(
              [
                FindPackageShare('visp_tk_tutorials'),
                'config',
                'mbt',
                'cube',
                'cube.json'
              ])
        ),
       DeclareLaunchArgument(
            "depth_camera_info_topic_name",
            description="Name of the depth camera topic.",
            default_value="/camera/camera/depth/camera_info"
        ),
        DeclareLaunchArgument(
            "depth_config_file",
            description="When using an XML file, path to the configuration file to initialize the depth tracker, if any. ``package://`` will be replaced by the path to the share folder of the corresponding package.",
            default_value=""
        ),
        DeclareLaunchArgument(
            "depth_image_topic_name",
            description="Name of the depth stream topic.",
            default_value="/camera/camera/depth/image_rect_raw"
        ),
        DeclareLaunchArgument(
            "depth_model_file",
            description="When using an XML file or not configuring the model for all trackers using a JSON file, this parameter becomes **REQUIRED** and must be set to the path towards the model file for the depth tracker, if there is one. ``package://`` will be replaced by the path to the share folder of the corresponding package.",
            default_value=""
        ),
        DeclareLaunchArgument(
            "detect_failure",
            description = "Permits to activate the detection of tracking failure based on the projection error.",
            default_value="true"
        ),
        DeclareLaunchArgument(
            "display_nb_frames_skipped",
            description = "This parameter indicates the number of frames skipped during display",
            default_value="-1"
        ),
        DeclareLaunchArgument(
            "headless_mode",
            description = "If True, the node will not display anything, expecting that another takes in charge the GUI.",
            default_value="False"
        ),
        DeclareLaunchArgument(
            'init_file',
            description="Absolute path towards the `.init` file that indicates the clicks location to initialize the MBT",
            default_value=PathJoinSubstitution(
              [
                FindPackageShare('visp_tk_tutorials'),
                'config',
                'mbt',
                'cube',
                'cube.init'
              ])
        ),
        DeclareLaunchArgument(
            "init_method",
            description="Initialization method to initialize the tracker. Default method is initialization by click.",
            default_value="click"
        ),
        DeclareLaunchArgument(
            "init_topic",
            description="If the ``init_method`` is set to topic, this parameter becomes **REQUIRED** and must be set to the topic of type ``geometry_msgs:msg::PoseStamped`` the tracker must use to get the init pose.",
            default_value=""
        ),
        ## [Launch argument for aligning streams, depth stream]
        DeclareLaunchArgument(
            "other_tracker",
            description="When set, the extrinsics will be loaded from a TF2 topic and this parameter must be a vector of size 2 such as [\"${OTHER_TRACKER_NAME}\",\"${OTHER_TRACKER_FRAME_NAME}\"]. The parameter ``reference_tracker`` becomes **REQUIRED**",
            default_value="[Depth,camera_depth_optical_frame]"
        ),
        ## [End launch argument for aligning streams, depth stream]
        DeclareLaunchArgument(
            "projection_error_threshold",
            description="Maximum tolerated projection error, if ``detect_failure`` is set to true.",
            default_value="30.0"
        ),
        ## [Launch argument for aligning streams, color stream]
        DeclareLaunchArgument(
            "reference_tracker",
            description="When set, the extrinsics will be loaded from a TF2 topic and this parameter must be a vector of size 2 such as [\"${REF_TRACKER_NAME}\",\"${REF_TRACKER_FRAME_NAME}\"]. The parameter ``other_tracker`` becomes **REQUIRED**.",
            default_value="[Color,camera_color_optical_frame]"
        ),
        ## [End launch argument for aligning streams, color stream]
        DeclareLaunchArgument(
            "rgb_camera_info_topic_name",
            description="Name of the color camera topic.",
            default_value="/camera/camera/color/camera_info"
        ),
        DeclareLaunchArgument(
            "rgb_model_file",
            description="When using an XML file or not configuring the model for all trackers using a JSON file, this parameter becomes **REQUIRED** and must be set to the path towards the model file for the RGB tracker. ``package://`` will be replaced by the path to the share folder of the corresponding package.",
            default_value=""
        ),
        DeclareLaunchArgument(
            "rgb_image_topic_name",
            description="Name of the color image topic.",
            default_value="/camera/camera/color/image_raw"
        ),
        DeclareLaunchArgument(
            "stream_qos_depth",
            description="The depth of the queue of both the RGB and depth image streams (they need to be the same) if depth is required.",
            default_value="5"
        ),
        DeclareLaunchArgument(
            "stream_qos_durability",
            description="The durability of both the RGB and depth image streams (they need to be the same) if depth is required.",
            default_value="volatile"
        ),
        DeclareLaunchArgument(
            "stream_qos_reliability",
            description="The reliability of both the RGB and depth image streams (they need to be the same) if depth is required.",
            default_value="reliable"
        ),
        DeclareLaunchArgument(
            "tracker_names",
            description="When using an XML file, this parameter becomes **REQUIRED**. It consists in an array of names for the different trackers (RGB and potentially depth) to use and must be of the same size than the parameter ``tracker_types``.",
            default_value="['']"
        ),
        DeclareLaunchArgument(
            "tracker_types",
            description="When using an XML file, this parameter becomes **REQUIRED**. It consists in an array of types of trackers to use and must be of the same size than the parameter ``tracker_names``. If a tracker must have several types (e.g. edge tracker + klt), the types name must be separated by a ``+`` (e.g. ``edge+klt`` is a valid value). RGB types cannot be mixed with depth types (e.g. ``edge+depthDense`` is not valid), they must be separated.",
            default_value="['']"
        ),
        DeclareLaunchArgument(
            "z_factor",
            description="Factor to convert the depth image expressed as uint16_t into meters. For instance, if a value of ``1000`` in the raw depth image corresponds to ``1 meter``, the ``z_factor`` must be set to ``0.001``.",
            default_value="0.001"
        ),
        DeclareLaunchArgument(
            "frame_size_ratio",
            description="Length of the axes of the displayed object frame, as a fraction of the object diameter (diagonal of the bounding box of the model, or of the tag). The axes are never shorter than 30 pixels.",
            default_value="0.3"
        ),
        DeclareLaunchArgument(
            "frame_thickness",
            description="Thickness, in pixels, of the axes of the displayed object frame.",
            default_value="2"
        ),
        SetEnvironmentVariable(name='GOMP_SPINCOUNT', value='0'), # To enable OpenMP acceleration
        SetEnvironmentVariable(name='RMW_FASTRTPS_PUBLICATION_MODE', value='ASYNCHRONOUS') # To avoid laggy rosbag
        ]
    )

    ld.add_action(OpaqueFunction(function=prepare_parameters))

    return ld
