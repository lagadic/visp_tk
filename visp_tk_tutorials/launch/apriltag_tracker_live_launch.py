from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, RegisterEventHandler, EmitEvent, LogInfo
from launch.events import Shutdown
from launch.event_handlers import (
    OnProcessExit
)
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():

    # ------------------------------------------------------------------ #
    #  Launch arguments                                                    #
    # ------------------------------------------------------------------ #
    declared_args = [
        # BEGIN_V4L2_ARGUMENTS
        DeclareLaunchArgument(
            "video_device",
            default_value="/dev/video0",
            description="Video device path (e.g. /dev/video0, /dev/video2 ...)",
        ),
        DeclareLaunchArgument(
            "camera_namespace",
            default_value="camera",
            description="Namespace for the v4l2_camera node",
        ),
        DeclareLaunchArgument(
            "camera_info_url",
            default_value="",
            description="URL to the camera calibration file (e.g. file:///path/to/camera.yaml).",
        ),
        # END_V4L2_ARGUMENTS
        # BEGIN_APRILTAG_ARGUMENTS
        DeclareLaunchArgument(
            "tag_family",
            default_value="36h11",
            description="AprilTag family (e.g. 36h11, 25h9, 16h5, circle21h7 ...)",
        ),
        DeclareLaunchArgument(
            "tag_size_keys",
            default_value="[-1]",
            description="List of tag IDs for which a size is specified. -1 = default for all.",
        ),
        DeclareLaunchArgument(
            "tag_size_values",
            default_value="[0.1]",
            description="Tag sizes in meters, matched by index with tag_size_keys.",
        ),
        DeclareLaunchArgument(
            "id_published",
            default_value="-1",
            description="ID of the tag whose pose is forwarded on /pose (-1 = none).",
        ),
        DeclareLaunchArgument(
            "pose_method",
            default_value="best_residual_virtual_vs",
            description="Pose estimation method",
        ),
        DeclareLaunchArgument(
            "display_tag",
            default_value="true",
            description="Display the detected tags in a ViSP window",
        ),
        # END_APRILTAG_ARGUMENTS
    ]

    # ------------------------------------------------------------------ #
    #  v4l2_camera node                                                    #
    # ------------------------------------------------------------------ #
    # BEGIN_V4L2_NODE
    # image_size must be a list of Python integers, this no LaunchConfiguration
    # for this parameter (the integer_array type is strict in ROS 2)
    v4l2_camera_node = Node(
        package="v4l2_camera",
        executable="v4l2_camera_node",
        name="v4l2_camera",
        namespace=LaunchConfiguration("camera_namespace"),
        parameters=[
            {
                "video_device": LaunchConfiguration("video_device"),
                "image_size": [1280, 720],   # native integers, no substitution
                "time_per_frame": [1, 30],   # idem : 1/30 s <=> 30 fps
                "camera_info_url": LaunchConfiguration("camera_info_url"),
            }
        ],
        output="screen",
    )
    # END_V4L2_NODE

    # ------------------------------------------------------------------ #
    #  AprilTag tracker node                                               #
    # ------------------------------------------------------------------ #
    # BEGIN_APRILTAG_NODE
    # The BaseTracker of visp_tracker_common use its own parameters for some topics
    # rgb_camera_info_topic_name  <=> camera_info topic
    # rgb_image_topic_name  <=> image topic
    apriltag_tracker_node = Node(
        package="visp_apriltag",
        executable="visp_apriltag_node",
        name="tracker_apriltag",
        parameters=[
            {
                "rgb_image_topic_name": "/camera/image_raw",
                "rgb_camera_info_topic_name": "/camera/camera_info",
                "tag_family": LaunchConfiguration("tag_family"),
                "tag_size_keys": LaunchConfiguration("tag_size_keys"),
                "tag_size_values": LaunchConfiguration("tag_size_values"),
                "id_published": LaunchConfiguration("id_published"),
                "pose_method": LaunchConfiguration("pose_method"),
                "display_tag": LaunchConfiguration("display_tag"),
            }
        ],
        output="screen",
    )
    # END_APRILTAG_NODE

    # BEGIN_SHUTDOWN
    shutdown_handler = RegisterEventHandler(
            OnProcessExit(
                target_action=apriltag_tracker_node,
                on_exit=[
                    LogInfo(msg=("The tracking node was closed, turning off the launch file")),
                    EmitEvent(event=Shutdown(
                        reason="tracking node closed"))
                ]
            )
        )
    # END_SHUTDOWN

    return LaunchDescription(declared_args + [v4l2_camera_node, apriltag_tracker_node, shutdown_handler])
