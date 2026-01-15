#include <visp_apriltag/AprilTagTracker.hpp>

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  std::shared_ptr<visp_apriltag::AprilTagTracker> tracker = std::make_shared<visp_apriltag::AprilTagTracker>("tracker_apriltag");
  bool success = tracker->init();
  if (success) {
    while (!tracker->has_to_quit()) {
      rclcpp::spin_some(tracker);
    }
  }
  return (success ? EXIT_SUCCESS : EXIT_FAILURE);
}
