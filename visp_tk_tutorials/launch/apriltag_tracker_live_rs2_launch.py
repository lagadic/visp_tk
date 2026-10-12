from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, RegisterEventHandler, EmitEvent, LogInfo
from launch.events import Shutdown
from launch.event_handlers import (
    OnProcessExit
)
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare

def generate_launch_description():

    # ------------------------------------------------------------------ #
    #  Launch arguments                                                    #
    # ------------------------------------------------------------------ #
    declared_args = [
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
            "enable_sync": "false",
            "enable_color": "true",
            "enable_depth": "false",
            "align_depth.enable": "false",
            "publish_tf": "true",# publish_tf permits to have the extrinsincs expressed as a TF2
            "rgb_camera.color_profile": "640,480,30",
            "depth_module.depth_profile": "0,0,0",
        }.items()
    )

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
                "rgb_image_topic_name": "/camera/camera/color/image_raw",
                "rgb_camera_info_topic_name": "/camera/camera/color/camera_info",
                "tag_family": LaunchConfiguration("tag_family"),
                "tag_size_keys": LaunchConfiguration("tag_size_keys"),
                "tag_size_values": LaunchConfiguration("tag_size_values"),
                "id_published": LaunchConfiguration("id_published"),
                "pose_method": LaunchConfiguration("pose_method"),
                "display_tag": LaunchConfiguration("display_tag"),
                "initial_tracking_status": True
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

    return LaunchDescription(declared_args + [realsense, apriltag_tracker_node, shutdown_handler])
