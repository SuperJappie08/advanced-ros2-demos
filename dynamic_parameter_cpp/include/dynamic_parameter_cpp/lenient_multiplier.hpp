// Copyright 2026 Jasper van Brakel
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

#ifndef DYNAMIC_PARAMETER_CPP__LENIENT_MULTIPLIER_HPP_
#define DYNAMIC_PARAMETER_CPP__LENIENT_MULTIPLIER_HPP_

#include <vector>

#include "dynamic_parameter_cpp/visibility_control.hpp"

#include "rclcpp/node.hpp"

#include "rcl_interfaces/msg/set_parameters_result.hpp"
#include "example_interfaces/msg/float64.hpp"

namespace dynamic_parameter_cpp
{

// TODO(SuperJappie08): DOCS
class LenientMultiplier : public rclcpp::Node {
public:
  DYNAMIC_PARAMETER_CPP_PUBLIC
  explicit LenientMultiplier(const rclcpp::NodeOptions & options);

private:
  OnSetParametersCallbackHandle::SharedPtr on_set_parameter_callback_handle_;
  PostSetParametersCallbackHandle::SharedPtr post_set_parameter_callback_handle_;

  rclcpp::Publisher<example_interfaces::msg::Float64>::SharedPtr multiplied_pub_;
  rclcpp::Subscription<example_interfaces::msg::Float64>::SharedPtr number_sub_;

  rcl_interfaces::msg::SetParametersResult validate_parameters_callback(
    const std::vector<rclcpp::Parameter> & parameters) const;

  void post_set_parameters_callback(
    const std::vector<rclcpp::Parameter> & parameters) const;

  void number_callback(const example_interfaces::msg::Float64::ConstSharedPtr number) const;
};

}  // namespace dynamic_parameter_cpp

#endif  // DYNAMIC_PARAMETER_CPP__LENIENT_MULTIPLIER_HPP_
