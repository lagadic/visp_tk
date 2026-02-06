string family
int32 id
float32 size # size of the tag, in meters
vision_msgs/Point2D center      # centre in (x,y) pixel coordinates
vision_msgs/Point2D[4] corners  # corners of tag ((x1,y1),(x2,y2),...)
geometry_msgs/PoseStamped pose
