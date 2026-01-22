#include <csignal>

#include <visp_apriltag/AprilTagTracker.hpp>

std::shared_ptr<visp_apriltag::AprilTagTracker> tracker;

void signalHandler(int signum)
{
  // Send a quit request to the tracker
  if (tracker) {
    tracker->stop_and_quit();
  }
  RCLCPP_WARN(tracker->get_logger(), "Sent termination signal to the node due to signal %d", signum);
}

int main(int argc, char *argv[])
{
  signal(SIGABRT, signalHandler);
  signal(SIGINT, signalHandler);
  signal(SIGTERM, signalHandler);

  rclcpp::init(argc, argv);
  tracker = std::make_shared<visp_apriltag::AprilTagTracker>("tracker_apriltag");
  bool success = tracker->init();
  if (success) {
    while (!tracker->has_to_quit()) {
      rclcpp::spin_some(tracker);
    }
  }
  return (success ? EXIT_SUCCESS : EXIT_FAILURE);
}
