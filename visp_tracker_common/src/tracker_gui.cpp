#include <rclcpp/rclcpp.hpp>

#include <visp_tracker_common/TrackerGUI.hpp>

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  std::shared_ptr<visp_tracker_common::TrackerGUI> gui = std::make_shared<visp_tracker_common::TrackerGUI>("tracker_gui");
  bool status = gui->init();
  if (status) {
    while (gui->has_to_run()) {
      rclcpp::spin_some(gui);
    }
  }
  rclcpp::shutdown();
  return (status ? EXIT_SUCCESS : EXIT_FAILURE);
}
