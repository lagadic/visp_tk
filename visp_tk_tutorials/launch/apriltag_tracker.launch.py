from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():

    # ------------------------------------------------------------------ #
    #  Launch arguments                                                    #
    # ------------------------------------------------------------------ #
    declared_args = [
        # --- v4l2_camera ---
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
        # --- AprilTag detector ---
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
            description="Tag sizes in metres, matched by index with tag_size_keys.",
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
            "rgb_camera_info_topic_name",
            default_value="/camera/image_raw",
            description="Topic name for the input image",
        ),
        DeclareLaunchArgument(
            "display_tag",
            default_value="false",
            description="Display the detected tags in a ViSP window",
        ),

    ]

    # ------------------------------------------------------------------ #
    #  v4l2_camera node                                                    #
    # ------------------------------------------------------------------ #
    # image_size doit être une liste d'entiers Python — pas de LaunchConfiguration
    # pour ce paramètre (le type integer_array est strict sous ROS 2)
    v4l2_camera_node = Node(
        package="v4l2_camera",
        executable="v4l2_camera_node",
        name="v4l2_camera",
        namespace=LaunchConfiguration("camera_namespace"),
        parameters=[
            {
                "video_device": LaunchConfiguration("video_device"),
                "image_size": [1280, 720],   # entiers natifs, pas de substitution
                "time_per_frame": [1, 30],   # idem : 1/30 s → 30 fps
                "camera_info_url": LaunchConfiguration("camera_info_url"),
            }
        ],
        output="screen",
    )

    # ------------------------------------------------------------------ #
    #  AprilTag tracker node                                               #
    # ------------------------------------------------------------------ #
    # Le BaseTracker de visp utilise ses propres paramètres de topic
    # rgb_camera_info_topic_name  → topic camera_info
    # rgb_image_topic_name  → topic image
    apriltag_tracker_node = Node(
        package="visp_apriltag",
        executable="visp_apriltag_node",
        name="tracker_apriltag",
        parameters=[
            {
                #"rgb_camera_info_topic_name": LaunchConfiguration("rgb_camera_info_topic_name"),
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

    return LaunchDescription(declared_args + [v4l2_camera_node, apriltag_tracker_node])
