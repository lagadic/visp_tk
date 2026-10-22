import yaml
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, RegisterEventHandler, SetEnvironmentVariable, GroupAction, EmitEvent, LogInfo, OpaqueFunction
from launch.events import Shutdown
from launch.event_handlers import (
    OnProcessExit
)
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare

def prepare_parameters(context):
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
                {'z_factor': z_factor}
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
    ## [Instanciating the node]
    mbt_node = Node(
        package="visp_mbt",
        executable="visp_mbt_node",
        name="mbt_node",
        output="screen",
        emulate_tty=True,
        parameters=parameters,
    )
    ## [Realsense camera]
    realsense = IncludeLaunchDescription(
        PathJoinSubstitution(
              [
                FindPackageShare('realsense2_camera'),
                'launch',
                'rs_launch.py'
              ]),
        launch_arguments={
            "enable_rgbd": "false",
            "enable_sync": "true",
            "enable_color": "true",
            "enable_depth": "true",
            "align_depth.enable": "false",
            "publish_tf": "true",# publish_tf permits to have the extrinsincs expressed as a TF2
            "rgb_camera.color_profile": "640,480,30",
            "depth_module.depth_profile": "640,480,30",
        }.items()
    )
    ## [Launching nodes]
    spawn_process = GroupAction([
      realsense,
      mbt_node
      ])
    ## [Handling shutdown]
    shutdown_handler = RegisterEventHandler(
            OnProcessExit(
                target_action=mbt_node,
                on_exit=[
                    LogInfo(msg=("The tracking node was closed, turning off the launch file")),
                    EmitEvent(event=Shutdown(
                        reason="tracking node closed"))
                ]
            )
        )

    return [spawn_process, shutdown_handler]

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
                'teabox',
                'teabox.json'
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
                'teabox',
                'teabox.init'
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
            default_value="best_effort"
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
        SetEnvironmentVariable(name='GOMP_SPINCOUNT', value='0'), # To enable OpenMP acceleration
        SetEnvironmentVariable(name='RMW_FASTRTPS_PUBLICATION_MODE', value='ASYNCHRONOUS') # To avoid laggy rosbag
        ]
    )

    ld.add_action(OpaqueFunction(function=prepare_parameters))

    return ld
