"""
Track the dragon with the RBT, the RealSense driver and the tracker being loaded as components
in the same (single-threaded) container with intra-process communication enabled.

Images are thus passed from the driver to the tracker without going through DDS, which permits to
run at the camera frame rate even with 640x480 images.

The container must be single-threaded (component_container): the Panda3D OpenGL context and the
ViSP X11 displays used by the tracker must always be used from the thread that created them, which a
multi-threaded executor does not guarantee. Blocking calls of the tracker (e.g. initialization by click)
do not stall the camera, since the RealSense driver publishes its frames from librealsense threads.
"""
import os

from ament_index_python.packages import get_package_share_directory, PackageNotFoundError
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, SetEnvironmentVariable, RegisterEventHandler, EmitEvent, LogInfo, OpaqueFunction
from launch.events import Shutdown
from launch.event_handlers import OnProcessExit
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


def generate_launch_description():
    config_file = LaunchConfiguration("config_file")
    depth_camera_info_topic_name = LaunchConfiguration("depth_camera_info_topic_name")
    depth_image_topic_name = LaunchConfiguration("depth_image_topic_name")
    display_nb_frames_skipped = LaunchConfiguration("display_nb_frames_skipped")
    headless_mode = LaunchConfiguration("headless_mode")
    init_file = LaunchConfiguration("init_file")
    init_method = LaunchConfiguration("init_method")
    init_topic = LaunchConfiguration("init_topic")
    model_file = LaunchConfiguration("model_file")
    rgb_camera_info_topic_name = LaunchConfiguration("rgb_camera_info_topic_name")
    rgb_image_topic_name = LaunchConfiguration("rgb_image_topic_name")
    stream_qos_depth = LaunchConfiguration("stream_qos_depth")
    stream_qos_durability = LaunchConfiguration("stream_qos_durability")
    stream_qos_reliability = LaunchConfiguration("stream_qos_reliability")
    z_factor = LaunchConfiguration("z_factor")
    rs_profile = LaunchConfiguration("rs_profile")
    intra_process = LaunchConfiguration("intra_process_comms")
    log_level = LaunchConfiguration("log_level")

    ld = LaunchDescription([
        DeclareLaunchArgument(
            'config_file',
            description="Absolute path towards the JSON config file for the RBT",
            default_value=PathJoinSubstitution(
                [FindPackageShare('visp_tk_tutorials'), 'config', 'rbt', 'configs', 'depth-ccd-mask.json'])
        ),
        DeclareLaunchArgument(
            "depth_camera_info_topic_name",
            description="Name of the depth camera topic.",
            default_value="/camera/camera/aligned_depth_to_color/camera_info"
        ),
        DeclareLaunchArgument(
            "depth_image_topic_name",
            description="Name of the depth stream topic.",
            default_value="/camera/camera/aligned_depth_to_color/image_raw"
        ),
        DeclareLaunchArgument(
            "display_nb_frames_skipped",
            description="This parameter indicates the number of frames skipped during display",
            default_value="-1"
        ),
        DeclareLaunchArgument(
            "headless_mode",
            description="If True, the node will not display anything, expecting that another takes in charge the GUI.",
            default_value="False"
        ),
        DeclareLaunchArgument(
            'init_file',
            description="Absolute path towards the `.init` file that indicates the clicks location to initialize the RBT",
            default_value=PathJoinSubstitution(
                [FindPackageShare('visp_tk_tutorials'), 'config', 'rbt', 'models', 'dragon', 'dragon.init'])
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
        DeclareLaunchArgument(
            "model_file",
            description="Path towards the model of the object to track.",
            default_value="package://visp_tk_tutorials/config/rbt/models/dragon/dragon.obj"
        ),
        DeclareLaunchArgument(
            "rgb_camera_info_topic_name",
            description="Name of the color camera topic.",
            default_value="/camera/camera/color/camera_info"
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
            description="The durability of both the RGB and depth image streams. Must be volatile for intra-process communication.",
            default_value="volatile"
        ),
        DeclareLaunchArgument(
            "stream_qos_reliability",
            description="The reliability of both the RGB and depth image streams (they need to be the same) if depth is required.",
            default_value="reliable"
        ),
        DeclareLaunchArgument(
            "z_factor",
            description="Factor to convert the depth image expressed as uint16_t into meters.",
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
        DeclareLaunchArgument(
            "rs_profile",
            description="RealSense color and depth profile 'width,height,fps'.",
            default_value="640,480,30"
        ),
        DeclareLaunchArgument(
            "intra_process_comms",
            description="Enable intra-process communication on the RealSense driver side, so that images are passed to the tracker without going through DDS.",
            default_value="True"
        ),
        DeclareLaunchArgument(
            "log_level",
            description="Log level of the tracker (debug, info, warn, error). debug shows the tracking time and the pose at each frame.",
            default_value="info"
        ),
        SetEnvironmentVariable(name='GOMP_SPINCOUNT', value='0'),  # To enable OpenMP acceleration
    ])

    realsense = ComposableNode(
        package='realsense2_camera',
        plugin='realsense2_camera::RealSenseNodeFactory',
        name='camera',
        namespace='camera',
        parameters=[{
            "camera_name": "camera",
            "enable_rgbd": False,
            "enable_sync": True,
            "enable_color": True,
            "enable_depth": True,
            "enable_infra1": False,
            "enable_infra2": False,
            "enable_gyro": False,
            "enable_accel": False,
            "align_depth.enable": True,
            "publish_tf": True,  # publish_tf permits to have the extrinsics expressed as a TF2
            "rgb_camera.color_profile": rs_profile,
            "rgb_camera.profile": rs_profile,  # name used by realsense2_camera <= 4.54
            "depth_module.depth_profile": rs_profile,
            "depth_module.profile": rs_profile,  # name used by realsense2_camera <= 4.54
        }],
        extra_arguments=[{'use_intra_process_comms': intra_process}],
    )

    rbt = ComposableNode(
        package='visp_rbt',
        plugin='visp_rbt::RBTTracker',
        name='tracker_rbt',
        parameters=[
            {'config_file': config_file},
            {'depth_camera_info_topic_name': depth_camera_info_topic_name},
            {'depth_image_topic_name': depth_image_topic_name},
            {'display_nb_frames_skipped': display_nb_frames_skipped},
            {'headless_mode': headless_mode},
            {'init_file': init_file},
            {'init_method': init_method},
            {'init_topic': init_topic},
            {'model_file': model_file},
            {'rgb_camera_info_topic_name': rgb_camera_info_topic_name},
            {'rgb_image_topic_name': rgb_image_topic_name},
            {'stream_qos_depth': stream_qos_depth},
            {'stream_qos_durability': stream_qos_durability},
            {'stream_qos_reliability': stream_qos_reliability},
            {'z_factor': z_factor},
            {'frame_size_ratio': ParameterValue(LaunchConfiguration('frame_size_ratio'), value_type=float)},
            {'frame_thickness': ParameterValue(LaunchConfiguration('frame_thickness'), value_type=int)},
        ],
        # No extra_arguments: the tracker enables intra-process on its image subscriptions only
    )

    container = ComposableNodeContainer(
        name='rbt_container',
        namespace='',
        package='rclcpp_components',
        executable='component_container',  # Must be single-threaded, see above
        composable_node_descriptions=[realsense, rbt],
        ros_arguments=['--log-level', ['tracker_rbt:=', log_level]],
        output='screen',
        emulate_tty=True,
    )

    shutdown_handler = RegisterEventHandler(
        OnProcessExit(
            target_action=container,
            on_exit=[
                LogInfo(msg="The container was closed, turning off the launch file"),
                EmitEvent(event=Shutdown(reason="container closed")),
            ]
        )
    )

    # Fail early, with an explicit message, if a file given as launch argument does not exist
    ld.add_action(OpaqueFunction(function=check_files_exist, args=['config_file', 'init_file', 'model_file']))
    ld.add_action(container)
    ld.add_action(shutdown_handler)
    return ld
