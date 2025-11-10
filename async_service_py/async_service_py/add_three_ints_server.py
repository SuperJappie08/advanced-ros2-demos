#!/usr/bin/env python3
# Copyright 2025 Jasper van Brakel
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

from advanced_demo_interfaces.srv import AddThreeInts
from example_interfaces.srv import AddTwoInts

import rclpy
from rclpy.callback_groups import ReentrantCallbackGroup
from rclpy.executors import ExternalShutdownException
from rclpy.node import Node

# In this example an 'advanced_demo_interfaces/AddThreeInts' server is implemented.
# This will be done by calling an add two ints server twice, in the following way:
#
#     a -\
#     b -+-\
#     c ---+-> sum


class AddThreeIntsServer(Node):

    def __init__(self) -> None:
        super().__init__('add_three_ints_server')
        # Override the private default callback group,
        # since the MutuallyExclusiveCallbackGroup cannot wait on itself.
        self._default_callback_group = ReentrantCallbackGroup()

        self._add_two_ints_client = self.create_client(AddTwoInts, 'add_two_ints')

        self._add_three_ints_server = self.create_service(
            AddThreeInts,
            'add_three_ints',
            self._add_three_ints_callback,
        )

    async def _add_three_ints_callback(self,
                                       req: AddThreeInts.Request,
                                       res: AddThreeInts.Response,
                                       ) -> AddThreeInts.Response:
        # Adding three ints, will be delegated to an add two ints server call twice.
        self.get_logger().info(f'Processing request: {{ a: {req.a}, b: {req.b}, c: {req.c} }}')

        req1 = AddTwoInts.Request(a=req.a, b=req.b)
        res1 = await self._add_two_ints_client.call_async(req1)  # type: AddTwoInts.Response
        self.get_logger().info(f"Received 'a + b = {res1.sum}'")

        req2 = AddTwoInts.Request(a=res1.sum, b=req.c)
        res2 = await self._add_two_ints_client.call_async(req2)  # type: AddTwoInts.Response
        self.get_logger().info(f"Received '(a + b) + c = {res2.sum}'")

        res.sum = res2.sum
        return res


def main(args=None):
    try:
        # NOTE: init context manager available starting from Kilted
        rclpy.init(args=args)
        add_three_ints_server = AddThreeIntsServer()

        rclpy.spin(add_three_ints_server)

    except (KeyboardInterrupt, ExternalShutdownException):
        pass
    finally:
        rclpy.try_shutdown()


if __name__ == '__main__':
    main()
