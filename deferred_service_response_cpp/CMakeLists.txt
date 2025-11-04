cmake_minimum_required(VERSION 3.20)
project(deferred_service_response_cpp)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(example_interfaces REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)

include_directories(include)

add_library(async_service_relay_component SHARED
  src/async_service_relay.cpp)
target_compile_definitions(async_service_relay_component
  PRIVATE "DEFERRED_SERVICE_RESPONSE_CPP_BUILDING_DLL")
target_link_libraries(async_service_relay_component PUBLIC
  rclcpp::rclcpp
  rclcpp_components::component
  ${example_interfaces_TARGETS})
rclcpp_components_register_node(async_service_relay_component
  PLUGIN "deferred_service_response_cpp::AsyncServiceRelay"
  EXECUTABLE async_service_relay_node)

# TODO(SuperJappie08): Look into ros2/demos:composition windows compatibility
# https://github.com/ros2/demos/blob/35a91bde49136adf677b27f16335a09c42e7d244/composition/CMakeLists.txt#L82-L87

install(TARGETS
  async_service_relay_component
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)

ament_export_dependencies(example_interfaces)
ament_export_dependencies(rclcpp)
ament_export_dependencies(rclcpp_components)

ament_export_include_directories("include/${PROJECT_NAME}")
install(DIRECTORY include/ DESTINATION include/${PROJECT_NAME})

ament_export_libraries(async_service_relay_component)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
