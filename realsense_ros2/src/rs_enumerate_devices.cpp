#include <rclcpp/rclcpp.hpp>
#include <librealsense2/rs.hpp>

class RSEnumerateDevices : public rclcpp::Node {
public:
  RSEnumerateDevices() : Node("rs_enumerate_devices") {
    // Empty constructor - we'll do the enumeration in main
  }
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<RSEnumerateDevices>();
  
  try {
    // Enumerate devices
    rs2::context ctx;
    auto devices = ctx.query_devices();
    RCLCPP_INFO(node->get_logger(), "%zu RealSense device(s) found:", devices.size());
    
    for (auto&& dev : devices) {
      RCLCPP_INFO(node->get_logger(), "  - %s, S/N: %s", 
        dev.get_info(RS2_CAMERA_INFO_NAME),
        dev.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER));
    }
  } catch (const std::exception &e) {
    RCLCPP_ERROR(node->get_logger(), "Exception: %s", e.what());
  }

  // Properly shutdown ROS 2
  rclcpp::shutdown();
  return 0;
} 