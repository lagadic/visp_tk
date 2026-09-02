std_msgs/Header header
string family
int32 id
float32 size # size of the tag, in meters
bool z_axis_aligned_with_camera_axis # If true, the z-axis of the tag frame is aligned with the camera frame
bool is_pose_valid # True if the pose could be computed, false otherwise
vision_msgs/Point2D center      # centre in (x,y) pixel coordinates
vision_msgs/Point2D[4] corners  # corners of tag ((x1,y1),(x2,y2),...)
geometry_msgs/Point[4] corners_3d  # 3D coordinates in the object frame of the corners of tag ((x1,y1,z1),(x2,y2,z2),...)
geometry_msgs/Pose pose
