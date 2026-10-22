std_msgs/Header header
string family                        # Tag family (36h11, 25h9, 16h5...)
int32 id                             # Tag id
float32 size                         # Size of the black tag border, in meters
bool z_axis_aligned_with_camera_axis # If true, the z-axis of the tag frame is aligned with the camera frame
bool is_pose_valid                   # True if the pose could be computed, false otherwise
vision_msgs/Point2D center           # Tag center of gravity pixel coordinates (x,y)
vision_msgs/Point2D[4] corners       # Tag corners pixel coordinates ((x1,y1),(x2,y2),...)
geometry_msgs/Point[4] corners_3d    # Tag corners 3D coordinates in the object frame ((x1,y1,z1),(x2,y2,z2),...)
geometry_msgs/Pose pose              # Tag pose (3D position and orientation)
