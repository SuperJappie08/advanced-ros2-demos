// Copyright 2025 Jasper van Brakel
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef ASYNC_SERVICE_SERVER_CPP__ASYNC_SERVICE_RELAY_HPP_
#define ASYNC_SERVICE_SERVER_CPP__ASYNC_SERVICE_RELAY_HPP_

#include <memory>

#include "async_service_server_cpp/visibility_control.hpp"

#include "example_interfaces/srv/add_two_ints.hpp"
#include "rclcpp/node.hpp"

namespace async_service_server_cpp
{

class AsyncServiceRelay : public rclcpp::Node {
public:
  ASYNC_SERVICE_SERVER_CPP_PUBLIC
  explicit AsyncServiceRelay(const rclcpp::NodeOptions & options);

private:
  rclcpp::Service<example_interfaces::srv::AddTwoInts>::SharedPtr service_;
  rclcpp::Client<example_interfaces::srv::AddTwoInts>::SharedPtr client_;

  void service_callback(
    const std::shared_ptr<rmw_request_id_t> header,
    const example_interfaces::srv::AddTwoInts::Request::SharedPtr req) const;
};

}  // namespace async_service_server_cpp

#endif  // ASYNC_SERVICE_SERVER_CPP__ASYNC_SERVICE_RELAY_HPP_
