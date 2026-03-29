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
from typing import Any
from typing import LiteralString

from example_interfaces.msg import String
from rcl_interfaces.msg import ParameterDescriptor

import rclpy
from rclpy.executors import ExternalShutdownException
from rclpy.node import Node
from rclpy.parameter import Parameter

MY_FIRST_DYNAMIC_PARAM_KEY = 'my_first_dynamic_parameter'
DEFAULT_DYNAMIC_PARAM_KEY = 'dynamic_parameter_with_default'


PARAM_DESCRIPTORS: dict[LiteralString, Any] = {
    MY_FIRST_DYNAMIC_PARAM_KEY: {
        'descriptor': ParameterDescriptor(
            description='This parameter can change type during execution',
            additional_constraints='This value should be interesting',
            # We need to set this flag to enable dynamic typing for this parameter
            dynamic_typing=True,
        )
    },
    DEFAULT_DYNAMIC_PARAM_KEY: {
        'value': 12,
        'descriptor': ParameterDescriptor(
            description='This parameter can change type during execution, but defaults to INTEGER',
            dynamic_typing=True,
        ),
    },
}


class DynamicParameterTalker(Node):

    def __init__(self) -> None:
        super().__init__('dynamic_parameter_talker')
        self.get_logger().info(f"Starting '{self.get_name()}'")

        self.add_post_set_parameters_callback(self.post_set_parameters_callback)

        self.declare_parameter('message_number', 0)

        # NOTE: Dynamic variables can be deleted as opposed to statically defined parameters.
        # These cannot be recreated unless `Node(..., allow_undeclared_parameters=True)`.
        # Setting this will also allow for arbitrary parameters to be excepted by the parameter
        #  services (which are used in for example `ros2param`)
        #
        # There are two options to prevent this:
        # - 1. Reject setting the parameter type to NOT_SET in the pre_set_parameter_callback
        #      This would mean the value cannot be reset to None (it can be set to another type)
        # - 2. Redeclare the parameter in the post_set_paramter callback
        #      This would allow for the parameter to be and reset to it's original state.
        #
        # In this demo there has been opted for option 2
        # To simplify the implementation most kwargs have been put in a dictionary
        self.declare_parameter(
            MY_FIRST_DYNAMIC_PARAM_KEY,
            **PARAM_DESCRIPTORS[MY_FIRST_DYNAMIC_PARAM_KEY],
        )

        self.declare_parameter(
            DEFAULT_DYNAMIC_PARAM_KEY,
            **PARAM_DESCRIPTORS[DEFAULT_DYNAMIC_PARAM_KEY],
        )

        self._chatter_pub = self.create_publisher(String, 'chatter', 10)
        self._publish_timer = self.create_timer(1.0, self.timer_callback)

    def post_set_parameters_callback(
        self, parameter_list: Sequence[Parameter]
    ) -> None:
        """
        React to valid parameter changes.

        :param parameter_list: The list of updated parameters.
        :type parameter_list: Sequence[Parameter[Any]]
        """
        logger = self.get_logger()

        for param in parameter_list:
            logger.info(f'Parameter {param.name!r} was updated.')

            if (
                param.type_ == Parameter.Type.NOT_SET
                and param.name in PARAM_DESCRIPTORS
                and not self.has_parameter(param.name)
            ):
                logger.warning(f'Parameter {param.name!r} has been recreated')
                self.declare_parameter(
                    param.name,
                    **PARAM_DESCRIPTORS[param.name],
                    # Explicitly ignore override to ensure fresh defaults are loaded
                    ignore_override=True,
                )
                continue

            if 'dynamic' in param.name:
                logger.info(f'{param.name!r} now has type {param.type_.name}')

    def timer_callback(self):
        msg = String()

        message_number_param = self.get_parameter('message_number')

        msg.data += f'Message number: {message_number_param.value}'

        for dyn_param_key in PARAM_DESCRIPTORS:
            dyn_param = self.get_parameter(dyn_param_key)
            msg.data += f'\n{dyn_param.name!r} is '
            if dyn_param.type_ != Parameter.Type.NOT_SET:
                msg.data += f'type {dyn_param.type_.name} and has value {dyn_param.value!r}'
            else:
                msg.data += 'not set!'

        self._chatter_pub.publish(msg)

        self.set_parameters([
            Parameter(
                name=message_number_param.name, value=message_number_param.value + 1
            )
        ])


def main(args=None) -> None:
    try:
        # NOTE: init context manager available starting from Kilted
        rclpy.init(args=args)
        dynamic_parameter_talker = DynamicParameterTalker()

        rclpy.spin(dynamic_parameter_talker)

    except (KeyboardInterrupt, ExternalShutdownException):
        pass
    finally:
        rclpy.try_shutdown()


if __name__ == '__main__':
    main()
