from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, ExecuteProcess, RegisterEventHandler, EmitEvent, LogInfo
from launch.events import Shutdown
from launch.event_handlers import (
    OnProcessExit
)
from launch.substitutions import LaunchConfiguration, FindExecutable, PathJoinSubstitution
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
            default_value="[-1,0,1]",
            description="List of tag IDs for which a size is specified. -1 = default for all.",
        ),
        DeclareLaunchArgument(
            "tag_size_values",
            default_value="[0.2315,0.2315,0.064]",
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
            "rgb_image_topic_name",
            default_value="/head_arm_rgbd/color/image_raw",
            description="Topic name for the input image",
        ),
        DeclareLaunchArgument(
              "rgb_camera_info_topic_name",
              default_value="/head_arm_rgbd/color/camera_info",
              description="Topic name for the camera parameters related to the input image",
          ),
        DeclareLaunchArgument(
            "display_tag",
            default_value="false",
            description="Display the detected tags in a ViSP window",
        ),
      # END_APRILTAG_ARGUMENTS
    ]

    # ------------------------------------------------------------------ #
    #  ROS2 bag node node                                                    #
    # ------------------------------------------------------------------ #
    # BEGIN_ROSBAG_PLAYER
    bag_folder = PathJoinSubstitution(
        [FindPackageShare("visp_tk_tutorials"), "bag", "apriltag","humble_short_apriltag_tutorial"]
    )
    bag_player = ExecuteProcess(
        cmd=[
            FindExecutable(name="ros2"),
            " bag",
            " play ",
            " --loop ",
            bag_folder
        ],
        output="screen",
        shell=True
      )
    # END_ROSBAG_PLAYER

    # ------------------------------------------------------------------ #
    #  AprilTag tracker node                                               #
    # ------------------------------------------------------------------ #
    # BEGIN_APRILTAG_NODE
    apriltag_tracker_node = Node(
        package="visp_apriltag",
        executable="visp_apriltag_node",
        name="tracker_apriltag",
        parameters=[
            {
                "rgb_camera_info_topic_name": LaunchConfiguration("rgb_camera_info_topic_name"),
                "rgb_image_topic_name": LaunchConfiguration("rgb_image_topic_name"),
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
    # BEGIN_APRILTAG_NODE

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

    return LaunchDescription(declared_args + [bag_player, apriltag_tracker_node, shutdown_handler])
