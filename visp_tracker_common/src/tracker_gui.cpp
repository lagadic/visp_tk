#include <csignal>

#include <rclcpp/rclcpp.hpp>

#include <visp_tracker_common/TrackerGUI.hpp>

std::shared_ptr<visp_tracker_common::TrackerGUI> gui;

void signalHandler(int signum)
{
  // Send a null velocity to the robot and set its state to quit
  if (gui) {
    gui->quit();
  }
  RCLCPP_WARN(gui->get_logger(), "Sent termination signal to the node due to signal %d", signum);
}

int main(int argc, char *argv[])
{
  signal(SIGABRT, signalHandler);
  signal(SIGINT, signalHandler);
  signal(SIGTERM, signalHandler);

  rclcpp::init(argc, argv);
  gui = std::make_shared<visp_tracker_common::TrackerGUI>("tracker_gui");
  bool status = gui->init(gui);
  if (status) {
    while (gui->has_to_run()) {
      rclcpp::spin_some(gui);
    }
  }
  rclcpp::shutdown();
  return (status ? EXIT_SUCCESS : EXIT_FAILURE);
}
