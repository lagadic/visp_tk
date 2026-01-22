#include <csignal>

#include <rclcpp/rclcpp.hpp>
#include <visp_mbt/MBTTracker.hpp>

std::shared_ptr<visp_mbt::MBTTracker> tracker;

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
  tracker = std::make_shared<visp_mbt::MBTTracker>("tracker_mbt");
  bool status = tracker->init();
  if (status) {
    while (!tracker->has_to_quit()) {
      rclcpp::spin_some(tracker);
    }
  }
  rclcpp::shutdown();
  return (status ? EXIT_SUCCESS : EXIT_FAILURE);
}
