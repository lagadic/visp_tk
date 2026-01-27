#include <csignal>

#include <rclcpp/rclcpp.hpp>

#include <visp_tracker_common/TrackerGUI.hpp>

std::shared_ptr<visp_tracker_common::TrackerGUI> gui;

void signalHandler(int signum)
{
  // Send a quit request to the tracker and set the GUI state to quit
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
  auto it_node = std::make_shared<rclcpp::Node>("it_node");
  bool status = gui->init(it_node);
  if (status) {
    while (gui->has_to_run()) {
      rclcpp::spin_some(it_node);
      rclcpp::spin_some(gui);
    }
  }

  if (it_node) {
    it_node.reset();
  }

  if (gui) {
    gui.reset();
  }
  rclcpp::shutdown();
  return (status ? EXIT_SUCCESS : EXIT_FAILURE);
}
