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

import math

from example_interfaces.msg import Float64
from rcl_interfaces.msg import FloatingPointRange
from rcl_interfaces.msg import ParameterDescriptor

import rclpy
from rclpy.executors import ExternalShutdownException
from rclpy.node import Node
from rclpy.parameter import Parameter


class NumberGenerator(Node):
    """A node to publish numbers at a fixed frequency."""

    def __init__(self) -> None:
        super().__init__('number_generator')

        frequency: float = self.declare_parameter(
            name='frequency',
            value=1.0,
            descriptor=ParameterDescriptor(
                description="The publishing frequency of the 'number' topic",
                read_only=True,
                floating_point_range=[
                    FloatingPointRange(
                        from_value=0.000001,
                        to_value=math.inf,
                    )
                ],
            ),
        ).value

        self.declare_parameter('number', 0.0)
        self.declare_parameter('increment', 0.1)

        self.number_publisher = self.create_publisher(Float64, 'number', 10)

        self.timer = self.create_timer(1.0 / frequency, self._timer_callback)

    def _timer_callback(self) -> None:
        number = self.get_parameter('number').value

        self.get_logger().info(f'Publishing: {number}')
        self.number_publisher.publish(Float64(data=number))

        increment = self.get_parameter('increment').value
        self.set_parameters([Parameter(name='number', value=number+increment)])


def main(args=None) -> None:
    try:
        # NOTE: init context manager available starting from Kilted
        rclpy.init(args=args)
        node = NumberGenerator()

        rclpy.spin(node)

    except (KeyboardInterrupt, ExternalShutdownException):
        pass
    finally:
        rclpy.try_shutdown()


if __name__ == '__main__':
    main()
