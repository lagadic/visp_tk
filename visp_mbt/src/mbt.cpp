#include <rclcpp/rclcpp.hpp>
#include <visp_mbt/MBTTracker.hpp>

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  std::shared_ptr<visp_mbt::MBTTracker> mbt = std::make_shared<visp_mbt::MBTTracker>("tracker_mbt");
  bool status = mbt->init();
  if (status) {
    while (!mbt->has_to_quit()) {
      rclcpp::spin_some(mbt);
    }
  }
  rclcpp::shutdown();
  return (status ? EXIT_SUCCESS : EXIT_FAILURE);
}
