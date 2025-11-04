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

#ifndef DEFERRED_SERVICE_RESPONSE_CPP__ASYNC_SERVICE_RELAY_HPP_
#define DEFERRED_SERVICE_RESPONSE_CPP__ASYNC_SERVICE_RELAY_HPP_

#include <memory>

#include "deferred_service_response_cpp/visibility_control.hpp"

#include "example_interfaces/srv/add_two_ints.hpp"
#include "rclcpp/node.hpp"

namespace deferred_service_response_cpp
{

// A add_two_ints service relay, which will relay the request and response to
// another add_two_ints server node.
class AsyncServiceRelay : public rclcpp::Node {
public:
  DEFERRED_SERVICE_RESPONSE_CPP_PUBLIC
  explicit AsyncServiceRelay(const rclcpp::NodeOptions & options);

private:
  rclcpp::Service<example_interfaces::srv::AddTwoInts>::SharedPtr service_;
  rclcpp::Client<example_interfaces::srv::AddTwoInts>::SharedPtr client_;

  // The service callback needs to be of one of the following types:
  //  - `SharedPtrDeferResponseCallback`
  //  - `SharedPtrDeferResponseCallbackWithServiceHandle`
  // This means it should accept the request header and the request itself.
  //
  // If the callback would also accept a response, then it is treated as
  // a normal non-deferred service callback.
  void service_callback(
    const std::shared_ptr<rmw_request_id_t> header,
    const example_interfaces::srv::AddTwoInts::Request::SharedPtr req) const;
};

}  // namespace deferred_service_response_cpp

#endif  // DEFERRED_SERVICE_RESPONSE_CPP__ASYNC_SERVICE_RELAY_HPP_
