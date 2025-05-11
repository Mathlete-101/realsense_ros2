cmake_minimum_required(VERSION 3.5)
project(realsense_ros2)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(tf2_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(pcl_conversions REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(image_transport REQUIRED)

# Find librealsense2
find_library(REALSENSE2_LIBRARY
  NAMES realsense2
  PATHS /usr/lib/x86_64-linux-gnu
  NO_DEFAULT_PATH
)

if(NOT REALSENSE2_LIBRARY)
  message(FATAL_ERROR "librealsense2 not found")
endif()

find_path(REALSENSE2_INCLUDE_DIR
  NAMES librealsense2/rs.hpp
  PATHS /usr/include
  NO_DEFAULT_PATH
)

if(NOT REALSENSE2_INCLUDE_DIR)
  message(FATAL_ERROR "librealsense2 headers not found")
endif()

find_package(Boost COMPONENTS system REQUIRED)
find_package(PCL COMPONENTS common REQUIRED)
find_package(PkgConfig)
pkg_search_module(Eigen3 REQUIRED eigen3)

find_package(tf2_geometry_msgs REQUIRED)
find_package(tf2_sensor_msgs REQUIRED)

set(DEPENDENCIES
  rclcpp
  geometry_msgs
  nav_msgs
  sensor_msgs 
  tf2_msgs
  tf2_ros
  tf2_geometry_msgs
  tf2_sensor_msgs
  pcl_conversions
  cv_bridge
  image_transport
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${REALSENSE2_INCLUDE_DIR}
  ${EIGEN_INCLUDE_DIRS}
)

# Install launch files.
install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}/
)

install(DIRECTORY
  config
  DESTINATION share/${PROJECT_NAME}/
)

## rs_t265_node
add_executable(rs_t265_node src/rs_t265_node.cpp)
ament_target_dependencies(rs_t265_node ${DEPENDENCIES})

target_include_directories(rs_t265_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

target_link_libraries(rs_t265_node
  ${REALSENSE2_LIBRARY}
)

install(TARGETS rs_t265_node
  DESTINATION lib/${PROJECT_NAME})

## rs_d435_node
add_executable(rs_d435_node 
  include/${PROJECT_NAME}/constants.hpp
  src/rs_d435_node.cpp)

ament_target_dependencies(rs_d435_node ${DEPENDENCIES})

target_link_libraries(rs_d435_node
  ${REALSENSE2_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}  # pcl_conversions requires this
  ${OpenCV_LIBRARIES}
  ${PCL_LIBRARIES}
  ${SDL2_LIBRARIES}
)

target_include_directories(rs_d435_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

install(TARGETS rs_d435_node
  DESTINATION lib/${PROJECT_NAME})

## rs_enumerate_devices
add_executable(rs_enumerate_devices src/rs_enumerate_devices.cpp)
ament_target_dependencies(rs_enumerate_devices ${DEPENDENCIES})

target_link_libraries(rs_enumerate_devices
  ${REALSENSE2_LIBRARY}
)

install(TARGETS rs_enumerate_devices
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package() 