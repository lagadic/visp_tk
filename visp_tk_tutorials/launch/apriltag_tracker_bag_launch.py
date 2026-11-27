"""
AprilTag tracker on the tutorial rosbag, the bag player and the tracker being loaded as components
in the same container.

Compared to apriltag_tracker_bag_launch.py, where `ros2 bag play` and the tracker run in two processes,
the images (about 1.2 MB each) do not go through the DDS transport between processes, which permits
to process them at the bag rate.

The container must be single-threaded (component_container): the ViSP X11 displays used by the tracker must
always be used from the thread that created them, which a multi-threaded executor does not guarantee.
"""
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, RegisterEventHandler, EmitEvent, LogInfo
from launch.events import Shutdown
from launch.event_handlers import OnProcessExit
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import ComposableNodeContainer
from launch_ros.descriptions import ComposableNode
from launch_ros.substitutions import FindPackageShare
from launch_ros.parameter_descriptions import ParameterValue


def generate_launch_description():
    declared_args = [
        # BEGIN_APRILTAG_ARGUMENTS
        DeclareLaunchArgument(
            "tag_family",
            default_value="36h11",
            description="AprilTag family (e.g. 36h11, 25h9, 16h5, circle21h7 ...)",
        ),
        DeclareLaunchArgument(
            "tag_size_keys",
            default_value="[-1,0,1]",
            description="List of tag IDs for which a size is specified. -1 = default for all.",
        ),
        DeclareLaunchArgument(
            "tag_size_values",
            default_value="[0.2315,0.2315,0.064]",
            description="Tag sizes in meters, in the same order as tag_size_keys.",
        ),
        DeclareLaunchArgument(
            "id_published",
            default_value="-1",
            description="ID of the tag whose pose is published, -1 to publish all of them.",
        ),
        DeclareLaunchArgument(
            "pose_method",
            default_value="best_residual_virtual_vs",
            description="Method used to compute the pose of the tags.",
        ),
        DeclareLaunchArgument(
            "rgb_image_topic_name",
            default_value="/head_arm_rgbd/color/image_raw",
            description="Name of the color image topic.",
        ),
        DeclareLaunchArgument(
            "rgb_camera_info_topic_name",
            default_value="/head_arm_rgbd/color/camera_info",
            description="Name of the color camera info topic.",
        ),
        DeclareLaunchArgument(
            "stream_qos_reliability",
            default_value="best_effort",
            choices=["best_effort", "reliable"],
            description="Reliability of the image stream subscription.",
        ),
        DeclareLaunchArgument(
            "display_tag",
            default_value="true",
            description="If true, the detected tags are displayed.",
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
            "headless_mode",
            default_value="false",
            description="If true, the tracker does not display anything.",
        ),
        DeclareLaunchArgument(
            "log_level",
            default_value="info",
            description="Log level of the tracker (debug, info, warn, error).",
        ),
        # END_APRILTAG_ARGUMENTS
    ]

    # BEGIN_ROSBAG_PLAYER
    bag_folder = PathJoinSubstitution(
        [FindPackageShare("visp_tk_tutorials"), "bag", "apriltag", "tutorial-apriltag"]
    )

    # rosbag2 player as a component, equivalent to `ros2 bag play --loop <bag_folder>`
    bag_player = ComposableNode(
        package="rosbag2_transport",
        plugin="rosbag2_transport::Player",
        name="player",
        parameters=[{
            "storage.uri": bag_folder,
            "play.loop": True,
            "play.disable_keyboard_controls": True,
        }],
    )

    # END_ROSBAG_PLAYER

    # BEGIN_APRILTAG_NODE
    apriltag_tracker = ComposableNode(
        package="visp_apriltag",
        plugin="visp_apriltag::AprilTagTracker",
        name="tracker_apriltag",
        parameters=[{
            "rgb_camera_info_topic_name": LaunchConfiguration("rgb_camera_info_topic_name"),
            "rgb_image_topic_name": LaunchConfiguration("rgb_image_topic_name"),
            "tag_family": LaunchConfiguration("tag_family"),
            "tag_size_keys": LaunchConfiguration("tag_size_keys"),
            "tag_size_values": LaunchConfiguration("tag_size_values"),
            "id_published": LaunchConfiguration("id_published"),
            "pose_method": LaunchConfiguration("pose_method"),
            "display_tag": LaunchConfiguration("display_tag"),
            "frame_size_ratio": ParameterValue(LaunchConfiguration("frame_size_ratio"), value_type=float),
            "frame_thickness": ParameterValue(LaunchConfiguration("frame_thickness"), value_type=int),
            "headless_mode": LaunchConfiguration("headless_mode"),
            "stream_qos_reliability": LaunchConfiguration("stream_qos_reliability"),
            "initial_tracking_status": True,
        }],
    )

    # END_APRILTAG_NODE

    # BEGIN_CONTAINER
    container = ComposableNodeContainer(
        name="apriltag_container",
        namespace="",
        package="rclcpp_components",
        executable="component_container",  # Must be single-threaded, see above
        composable_node_descriptions=[bag_player, apriltag_tracker],
        ros_arguments=["--log-level", ["tracker_apriltag:=", LaunchConfiguration("log_level")]],
        output="screen",
        emulate_tty=True,
    )

    # END_CONTAINER

    # BEGIN_SHUTDOWN
    shutdown_handler = RegisterEventHandler(
        OnProcessExit(
            target_action=container,
            on_exit=[
                LogInfo(msg="The container was closed, turning off the launch file"),
                EmitEvent(event=Shutdown(reason="container closed")),
            ]
        )
    )

    # END_SHUTDOWN

    return LaunchDescription(declared_args + [container, shutdown_handler])
