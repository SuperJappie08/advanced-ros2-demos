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

#include "deferred_service_response_cpp/add_three_ints_server.hpp"

#include "rclcpp/logging.hpp"
#include "rclcpp_components/register_node_macro.hpp"

#include "example_interfaces/srv/add_two_ints.hpp"

using namespace std::placeholders;

namespace deferred_service_response_cpp
{

AddThreeIntsServer::AddThreeIntsServer(const rclcpp::NodeOptions & options)
: Node("add_three_ints_server", options)
{
  RCLCPP_INFO(get_logger(), "Starting the add three ints server");

  add_two_ints_client_ = this->create_client<AddTwoInts>("add_two_ints");

  add_three_ints_service_ = this->create_service<AddThreeInts>("add_three_ints",
      std::bind(&AddThreeIntsServer::add_three_ints_service_callback, this, _1, _2));
}

// The deferred response callback.
// In this situation it is easiest to attach a callback to our service client,
// in which the response of this service callback will be send.
void AddThreeIntsServer::add_three_ints_service_callback(
  const std::shared_ptr<rmw_request_id_t> req_id,
  const AddThreeInts::Request::SharedPtr req) const
{
  RCLCPP_INFO(get_logger(), "Processing request %ld: { a: %ld, b: %ld, c: %ld }",
              req_id->sequence_number, req->a, req->b, req->c);

  // In order to not block, we need to add nested callbacks to the clients
  // response. To make this slightly easier, a convenience function was made.
  this->add_two_ints(req->a, req->b, [this, req_id, req](int64_t sum) {
      RCLCPP_INFO(this->get_logger(), "Received 'a + b = %ld'", sum);

      this->add_two_ints(sum, req->c, [this, req_id](int64_t sum) {
        RCLCPP_INFO(this->get_logger(), "Received '(a + b) + c = %ld'", sum);
        AddThreeInts::Response res;
        res.sum = sum;

          // NOTE: We are responsible for sending a response, since this uses a deferred response.
          //       This will always happen as long as the other service eventually responds.
        this->add_three_ints_service_->send_response(*req_id, res);
    });
  });
}

void AddThreeIntsServer::add_two_ints(
  int64_t a, int64_t b,
  std::function<void(int64_t)> cb) const
{
  auto request = std::make_shared<AddTwoInts::Request>();
  request->a = a;
  request->b = b;

  this->add_two_ints_client_->async_send_request(
      request, [cb](AddTwoIntsResponseFuture future) {
      auto result = future.get();
      cb(result->sum);
      });
}

}  // namespace deferred_service_response_cpp

RCLCPP_COMPONENTS_REGISTER_NODE(deferred_service_response_cpp::AddThreeIntsServer)
