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

#include <functional>
#include <sstream>
#include <vector>

#include "dynamic_parameter_cpp/lenient_multiplier.hpp"

#include "rclcpp/logging.hpp"
#include "rclcpp_components/register_node_macro.hpp"

#include "example_interfaces/msg/float64.hpp"
#include "rcl_interfaces/msg/parameter_descriptor.hpp"
#include "rcl_interfaces/msg/set_parameters_result.hpp"

using namespace std::placeholders;
using namespace std::chrono_literals;

namespace dynamic_parameter_cpp
{

LenientMultiplier::LenientMultiplier(const rclcpp::NodeOptions & options)
: Node("lenient_multiplier", options)
{
  using rcl_interfaces::msg::ParameterDescriptor;

  // A parameter to control if the demo node is lenient or strict
  auto is_lenient_descriptor = ParameterDescriptor();
  is_lenient_descriptor.description =
    "Toggle if the 'multiplier' parameter is lenient";
  is_lenient_descriptor.read_only = true;
  bool is_lenient =
    declare_parameter("is_lenient", true, is_lenient_descriptor);

  if (is_lenient) {
    RCLCPP_INFO(get_logger(), "The 'multiplier' parameter is lenient");
    on_set_parameter_callback_handle_ = add_on_set_parameters_callback(
        std::bind(&LenientMultiplier::validate_parameters_callback, this, _1));
  } else {
    RCLCPP_WARN(get_logger(),
                "The 'multiplier' parameter is statically typed!");
  }

  // Add a post set callback so changes can bne logged easily
  post_set_parameter_callback_handle_ =
    add_post_set_parameters_callback(std::bind(&LenientMultiplier::post_set_parameters_callback,
      this, _1));

  auto multiplier_descriptor = ParameterDescriptor();
  multiplier_descriptor.dynamic_typing = is_lenient;
  if (is_lenient) {
    multiplier_descriptor.additional_constraints =
      "Must be a valid numeric type ('double' or 'integer')";
  }
  declare_parameter("multiplier", 1.0, multiplier_descriptor);

  multiplied_pub_ = create_publisher<example_interfaces::msg::Float64>("multiplied_number", 10);
  number_sub_ = create_subscription<example_interfaces::msg::Float64>("number", 10,
      std::bind(&LenientMultiplier::number_callback, this, _1));
}

rcl_interfaces::msg::SetParametersResult
LenientMultiplier::validate_parameters_callback(
  const std::vector<rclcpp::Parameter> & parameters) const
{
  auto result = rcl_interfaces::msg::SetParametersResult();

  for (const auto & param : parameters) {
    if (const auto name = param.get_name(); name == "multiplier") {
      if (const auto param_type = param.get_type();
        param_type != rclcpp::PARAMETER_DOUBLE &&
        param_type != rclcpp::PARAMETER_INTEGER)
      {
        std::ostringstream reason;
        reason << "'" << name
               << "' must be a numeric type ('double' or 'integer'), but was of type '"
               << param_type << "'";

        result.successful = false;
        result.reason = reason.str();

        return result;
      }
    }
  }

  result.successful = true;
  return result;
}

void LenientMultiplier::post_set_parameters_callback(
  const std::vector<rclcpp::Parameter> & parameters) const
{
  for (const auto & param : parameters) {
    if (const auto name = param.get_name(); name == "multiplier") {
      RCLCPP_INFO_STREAM(get_logger(),
          "The '" << name << "' parameter has been set to '" <<
          rclcpp::to_string(param.get_parameter_value()) << "'");
    }
  }
}

void LenientMultiplier::number_callback(
  const example_interfaces::msg::Float64::ConstSharedPtr number) const
{
  auto multiplier_param = get_parameter("multiplier");

  RCLCPP_INFO_STREAM(get_logger(),
      "Multiplying " << number->data << " with multiplier " <<
      rclcpp::to_string(multiplier_param.get_parameter_value()));

  double multiplier;
  if (multiplier_param.get_type() == rclcpp::PARAMETER_DOUBLE) {
    multiplier = multiplier_param.as_double();
  } else {
    multiplier = static_cast<double>(multiplier_param.as_int());
  }

  auto msg = example_interfaces::msg::Float64();
  msg.data = multiplier * number->data;
  multiplied_pub_->publish(msg);
}

}  // namespace dynamic_parameter_cpp

RCLCPP_COMPONENTS_REGISTER_NODE(dynamic_parameter_cpp::LenientMultiplier)
