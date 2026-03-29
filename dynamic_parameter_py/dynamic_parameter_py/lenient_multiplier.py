# Copyright 2026 Jasper van Brakel
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from collections.abc import Sequence

from example_interfaces.msg import Float64
from rcl_interfaces.msg import ParameterDescriptor
from rcl_interfaces.msg import SetParametersResult

import rclpy
from rclpy.executors import ExternalShutdownException
from rclpy.node import Node
from rclpy.parameter import Parameter

NUMERIC_TYPES = {Parameter.Type.DOUBLE, Parameter.Type.INTEGER}


class LenientMultiplier(Node):
    """A multiplier node with a double parameter which also excepts ints."""

    def __init__(self) -> None:
        super().__init__('lenient_multiplier')

        # Add a parameter to control if the demo is lenient or strict
        is_lenient: bool = self.declare_parameter(
            'is_lenient',
            True,
            descriptor=ParameterDescriptor(
                description="Toggle if the 'multiplier' parameter is lenient",
                read_only=True,
            ),
        ).value

        if is_lenient:
            self.get_logger().info("The 'multiplier' parameter is lenient")
            self.add_on_set_parameters_callback(self._validate_parameters_callback)
        else:
            self.get_logger().warning("The 'multiplier' parameter is statically typed!")

        self.add_post_set_parameters_callback(self._post_set_parameters_callback)

        self.declare_parameter('multiplier', value=1.0, descriptor=ParameterDescriptor(
            dynamic_typing=is_lenient,
            additional_constraints=('Must be a valid numeric type (DOUBLE or INTEGER)'
                                    if is_lenient else None)
        ))

        self._multiplied_pub = self.create_publisher(Float64, 'multiplied_number', 10)
        self._number_sub = self.create_subscription(Float64, 'number', self._number_callback, 10)

    def _validate_parameters_callback(self, params: Sequence[Parameter]) -> SetParametersResult:
        for param in params:
            # Reject updates to the multiplier parameter if the type is not numeric
            if param.name == 'multiplier' and param.type_ not in NUMERIC_TYPES:
                return SetParametersResult(
                    successful=False,
                    reason=(
                        "'multiplier' must be a numeric type (DOUBLE or INTEGER), "
                        f"but was of type '{param.type_.name}'"
                    )
                )

        return SetParametersResult(successful=True)

    def _post_set_parameters_callback(self, params: Sequence[Parameter]) -> None:
        for param in params:
            if param.name == 'multiplier':
                self.get_logger().info(f'The multiplier has been set to {param.value!r}')

    def _number_callback(self, number: Float64) -> None:
        # No extra conversion is necessary as Python implicitly casts numeric
        # types when necessary in multiplication.
        multiplier = self.get_parameter('multiplier').value

        self.get_logger().info(f'Multiplying {number.data!r} with multiplier {multiplier!r}')

        msg = Float64(data=number.data*multiplier)
        self._multiplied_pub.publish(msg)


def main(args=None) -> None:
    try:
        # NOTE: init context manager available starting from Kilted
        rclpy.init(args=args)
        node = LenientMultiplier()

        rclpy.spin(node)

    except (KeyboardInterrupt, ExternalShutdownException):
        pass
    finally:
        rclpy.try_shutdown()


if __name__ == '__main__':
    main()
