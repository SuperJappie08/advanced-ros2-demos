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

#ifndef DEFERRED_SERVICE_RESPONSE_CPP__ADD_THREE_INTS_SERVER_HPP_
#define DEFERRED_SERVICE_RESPONSE_CPP__ADD_THREE_INTS_SERVER_HPP_

#include <memory>
#include <functional>

#include "deferred_service_response_cpp/visibility_control.hpp"

#include "advanced_demo_interfaces/srv/add_three_ints.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"
#include "rclcpp/node.hpp"

namespace deferred_service_response_cpp
{

// A add_two_ints service relay, which will relay the request and response to
// another add_two_ints server node.
class AddThreeIntsServer : public rclcpp::Node {
public:
  DEFERRED_SERVICE_RESPONSE_CPP_PUBLIC
  explicit AddThreeIntsServer(const rclcpp::NodeOptions & options);

  using AddTwoInts = example_interfaces::srv::AddTwoInts;
  using AddThreeInts = advanced_demo_interfaces::srv::AddThreeInts;
  using AddTwoIntsResponseFuture = rclcpp::Client<AddTwoInts>::SharedFuture;

private:
  rclcpp::Service<AddThreeInts>::SharedPtr add_three_ints_service_;
  rclcpp::Client<AddTwoInts>::SharedPtr add_two_ints_client_;

  // The service callback needs to be of one of the following types:
  //  - `SharedPtrDeferResponseCallback`
  //  - `SharedPtrDeferResponseCallbackWithServiceHandle`
  // This means it should at least accept a request id and the request itself.
  //
  // If the callback would also accept a response, then it is treated as
  // a normal non-deferred service callback.
  //
  // NOTE: The request id is sometimes referred to as the request header.
  void add_three_ints_service_callback(
    const std::shared_ptr<rmw_request_id_t> req_id,
    const AddThreeInts::Request::SharedPtr req) const;

  // Add a convenience function to make chaining requests easier.
  void add_two_ints(int64_t a, int64_t b, std::function<void(int64_t)> cb) const;
};

}  // namespace deferred_service_response_cpp

#endif  // DEFERRED_SERVICE_RESPONSE_CPP__ADD_THREE_INTS_SERVER_HPP_
