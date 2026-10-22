from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, SetEnvironmentVariable, RegisterEventHandler, EmitEvent, LogInfo , OpaqueFunction
from launch.events import Shutdown
from launch.event_handlers import (
    OnProcessExit
)
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare

init_pose_topic_name = "/other_tracker/pose"

def handle_object_parameter(context):
  ## [Getting the values of the tracker-related launch arguments]
  config_file = LaunchConfiguration("config_file")
  depth_camera_info_topic_name = LaunchConfiguration("depth_camera_info_topic_name")
  depth_image_topic_name = LaunchConfiguration("depth_image_topic_name")
  display_nb_frames_skipped = LaunchConfiguration("display_nb_frames_skipped")
  headless_mode = LaunchConfiguration("headless_mode")
  init_file = LaunchConfiguration("init_file")
  init_method = LaunchConfiguration("init_method")
  init_topic  = LaunchConfiguration("init_topic")
  rgb_camera_info_topic_name = LaunchConfiguration("rgb_camera_info_topic_name")
  rgb_image_topic_name = LaunchConfiguration("rgb_image_topic_name")
  stream_qos_depth = LaunchConfiguration("stream_qos_depth")
  stream_qos_durability = LaunchConfiguration("stream_qos_durability")
  stream_qos_reliability = LaunchConfiguration("stream_qos_reliability")
  z_factor = LaunchConfiguration("z_factor")
  ## [Handling tracked object]
  object_name = LaunchConfiguration("object_name").perform(context)
  if object_name == "dragon":
    model_file = "package://visp_tk_tutorials/config/rbt/models/dragon/dragon.obj"
  elif object_name == "cube":
     model_file = "package://visp_tk_tutorials/config/rbt/models/cube/cube.obj"
  elif object_name == "stomach":
       model_file = "package://visp_tk_tutorials/config/rbt/models/stomach/stomach.obj"
  else:
     raise RuntimeError(f"Unexpected object_name {object_name}")
  ## [Sequence player]
  sequence_folder = PathJoinSubstitution(
      [FindPackageShare("visp_tk_tutorials"), "sequence"]
  )
  sequence_player = Node(
      package='visp_tk_tutorials',
      # namespace="rbt_ns",
      executable='sequence_player_node',
      name='sequence_player',
      output='screen',
      emulate_tty=True,
      parameters=[
          {'sequence_path' : sequence_folder},
          {'object_name': object_name},
          {'depth_image_topic_name': depth_image_topic_name},
          {'depth_camera_info_topic_name':depth_camera_info_topic_name},
          {'rgb_image_topic_name':rgb_image_topic_name},
          {'rgb_camera_info_topic_name':rgb_camera_info_topic_name},
          {'init_pose_topic_name': init_pose_topic_name}
      ]
    )
  ## [RBT tracker node]
  rbt_node = Node(
      package='visp_rbt',
      # namespace="rbt_ns",
      executable='visp_rbt_node',
      name='tracker_rbt',
      output='screen',
      emulate_tty=True,
      parameters=[
          {'config_file' : config_file},
          {'depth_camera_info_topic_name' : depth_camera_info_topic_name},
          {'depth_image_topic_name' : depth_image_topic_name},
          {'display_nb_frames_skipped' : display_nb_frames_skipped},
          {'headless_mode' : headless_mode},
          {'init_file' : init_file},
          {'init_method' : init_method},
          {'init_topic' : init_topic},
          {'model_file': model_file},
          {'rgb_camera_info_topic_name' : rgb_camera_info_topic_name},
          {'rgb_image_topic_name' : rgb_image_topic_name},
          {'stream_qos_depth' : stream_qos_depth},
          {'stream_qos_durability' : stream_qos_durability},
          {'stream_qos_reliability' : stream_qos_reliability},
          {'z_factor' : z_factor}
      ]
  )
  ## [Shutdown-event handler]
  shutdown_handler = RegisterEventHandler(
          OnProcessExit(
              target_action=rbt_node,
              on_exit=[
                  LogInfo(msg=("The tracking node was closed, turning off the launch file")),
                  EmitEvent(event=Shutdown(
                      reason="tracking node closed"))
              ]
          )
      )
  ## [Returning launch description items]
  return [sequence_player, rbt_node, shutdown_handler]

def generate_launch_description():
    ## [Declaring launch arguments]
    ld = LaunchDescription( [
       ## [RBT arguments]
        DeclareLaunchArgument(
            'config_file',
            description="Absolute path towards the JSON config file for the RBT",
            default_value=PathJoinSubstitution(
              [
                FindPackageShare('visp_tk_tutorials'),
                'config',
                'rbt',
                'configs',
                'depth-ccd-mask.json'
              ])
        ),
        DeclareLaunchArgument(
            "depth_camera_info_topic_name",
            description="Name of the depth camera topic.",
            default_value="/camera_rgb/depth/camera_info"
        ),
        DeclareLaunchArgument(
            "depth_image_topic_name",
            description="Name of the depth stream topic.",
            default_value="/camera_rgb/depth/image_raw"
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
            description="Absolute path towards the `.init` file that indicates the clicks location to initialize the RBT",
            default_value=PathJoinSubstitution(
              [
                FindPackageShare('visp_tk_tutorials'),
                'config',
                'rbt',
                'models',
                'dragon',
                'dragon.init'
              ])
        ),
        DeclareLaunchArgument(
            "init_method",
            description="Initialization method to initialize the tracker. Default method is initialization by click.",
            default_value="topic"
        ),
        DeclareLaunchArgument(
            "init_topic",
            description="If the ``init_method`` is set to topic, this parameter becomes **REQUIRED** and must be set to the topic of type ``geometry_msgs:msg::PoseStamped`` the tracker must use to get the init pose.",
            default_value=init_pose_topic_name
        ),
        DeclareLaunchArgument(
            "rgb_camera_info_topic_name",
            description="Name of the color camera topic.",
            default_value="/camera_rgb/color/camera_info"
        ),
        DeclareLaunchArgument(
            "rgb_image_topic_name",
            description="Name of the color image topic.",
            default_value="/camera_rgb/color/image_raw"
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
            "z_factor",
            description="Factor to convert the depth image expressed as uint16_t into meters. For instance, if a value of ``1000`` in the raw depth image corresponds to ``1 meter``, the ``z_factor`` must be set to ``0.001``.",
            default_value="0.001"
        ),
      ## [Sequence-player arguments]
      DeclareLaunchArgument(
          "object_name",
          description="Name of the object to track in the sequence.",
          default_value="dragon",
          choices=["dragon","cube","stomach"]
      ),
      ## [Setting OpenMP environment variables]
        SetEnvironmentVariable(name='GOMP_SPINCOUNT', value='0'), # To enable OpenMP acceleration
    ])

    ld.add_action(OpaqueFunction(function=handle_object_parameter))

    return ld
