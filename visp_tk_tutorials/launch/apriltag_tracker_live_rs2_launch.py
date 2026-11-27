from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, RegisterEventHandler, EmitEvent, LogInfo
from launch.events import Shutdown
from launch.event_handlers import (
    OnProcessExit
)
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import ComposableNodeContainer
from launch_ros.descriptions import ComposableNode
from launch_ros.parameter_descriptions import ParameterValue

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
        # END_APRILTAG_ARGUMENTS
    ]

    ## [Realsense camera]
    # Same settings as rs_launch.py would get, the node being loaded as a component instead
    realsense = ComposableNode(
        package="realsense2_camera",
        plugin="realsense2_camera::RealSenseNodeFactory",
        name="camera",
        namespace="camera",
        parameters=[
            {
                "camera_name": "camera",
                "enable_rgbd": False,
                "enable_sync": False,
                "enable_color": True,
                "enable_depth": False,
                "enable_infra1": False,
                "enable_infra2": False,
                "enable_gyro": False,
                "enable_accel": False,
                "align_depth.enable": False,
                "publish_tf": True,  # publish_tf permits to have the extrinsincs expressed as a TF2
                "rgb_camera.color_profile": "640,480,30",
                "rgb_camera.profile": "640,480,30",  # name used by realsense2_camera <= 4.54
                "depth_module.depth_profile": "0,0,0",
                "depth_module.profile": "0,0,0",  # name used by realsense2_camera <= 4.54
            }
        ],
        # The images are passed to the tracker without going through DDS
        extra_arguments=[{"use_intra_process_comms": True}],
    )

    # ------------------------------------------------------------------ #
    #  AprilTag tracker component                                          #
    # ------------------------------------------------------------------ #
    # BEGIN_APRILTAG_NODE
    # The BaseTracker of visp_tracker_common use its own parameters for some topics
    # rgb_camera_info_topic_name  <=> camera_info topic
    # rgb_image_topic_name  <=> image topic
    # The tracker enables intra-process communication on its image subscription by itself
    apriltag_tracker_node = ComposableNode(
        package="visp_apriltag",
        plugin="visp_apriltag::AprilTagTracker",
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
                "frame_size_ratio": ParameterValue(LaunchConfiguration("frame_size_ratio"), value_type=float),
                "frame_thickness": ParameterValue(LaunchConfiguration("frame_thickness"), value_type=int),
                "initial_tracking_status": True
            }
        ],
    )
    # END_APRILTAG_NODE

    # ------------------------------------------------------------------ #
    #  Component container                                                 #
    # ------------------------------------------------------------------ #
    # BEGIN_CONTAINER
    # Both components are loaded in the same process, so that the images (640x480 RGB, about 1 MB each)
    # do not go through DDS. The container must be single-threaded: the ViSP displays used by the tracker
    # must always be used from the thread that created them.
    container = ComposableNodeContainer(
        name="apriltag_container",
        namespace="",
        package="rclcpp_components",
        executable="component_container",
        composable_node_descriptions=[realsense, apriltag_tracker_node],
        output="screen",
        emulate_tty=True,
    )
    # END_CONTAINER

    # BEGIN_SHUTDOWN
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
    # END_SHUTDOWN

    return LaunchDescription(declared_args + [container, shutdown_handler])
