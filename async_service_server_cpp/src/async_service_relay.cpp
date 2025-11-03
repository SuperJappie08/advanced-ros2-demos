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

#include <functional>
#include <memory>

#include "async_service_server_cpp/async_service_relay.hpp"

#include "rclcpp/logging.hpp"
#include "rclcpp_components/register_node_macro.hpp"

#include "example_interfaces/srv/add_two_ints.hpp"

using namespace std::placeholders;

namespace async_service_server_cpp
{

AsyncServiceRelay::AsyncServiceRelay(const rclcpp::NodeOptions & options)
: Node("async_service_relay", options)
{
  client_ =
    this->create_client<example_interfaces::srv::AddTwoInts>("add_two_ints");
  service_ = this->create_service<example_interfaces::srv::AddTwoInts>(
      "~/add_two_ints",
      std::bind(&AsyncServiceRelay::service_callback, this, _1, _2));
}

void AsyncServiceRelay::service_callback(
  const std::shared_ptr<rmw_request_id_t> header,
  const example_interfaces::srv::AddTwoInts::Request::SharedPtr req) const
{
  RCLCPP_INFO(get_logger(), "Deferring service callback %ld",
              header->sequence_number);

  using ServiceResponseFuture =
    rclcpp::Client<example_interfaces::srv::AddTwoInts>::SharedFuture;
  auto client_callback = [this, header](ServiceResponseFuture future) {
      auto response = future.get();
      RCLCPP_INFO(this->get_logger(), "Response received for %ld",
                header->sequence_number);

      service_->send_response(*header, *response);
    };

  client_->async_send_request(req, client_callback);
}

}  // namespace async_service_server_cpp

RCLCPP_COMPONENTS_REGISTER_NODE(async_service_server_cpp::AsyncServiceRelay)
