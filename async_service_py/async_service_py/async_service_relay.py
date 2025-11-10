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

from example_interfaces.srv import AddTwoInts

import rclpy
from rclpy.callback_groups import ReentrantCallbackGroup
from rclpy.executors import ExternalShutdownException
from rclpy.node import Node


class AsyncServiceRelay(Node):

    def __init__(self) -> None:
        super().__init__('service_relay')
        self._callback_group = ReentrantCallbackGroup()

        self._client = self.create_client(
            AddTwoInts,
            'add_two_ints',
            callback_group=self._callback_group
        )

        while not self._client.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('service not available, waiting again...')

        self._service = self.create_service(
            AddTwoInts,
            '~/add_two_ints',
            self._service_callback,
            callback_group=self._callback_group
        )

    async def _service_callback(self,
                                req: AddTwoInts.Request,
                                res: AddTwoInts.Response
                                ) -> AddTwoInts.Response:
        self.get_logger().info('Request received: %s' % req)
        response: AddTwoInts.Response = await self._client.call_async(req)
        self.get_logger().info('Response received for %s' % req)
        res.sum = response.sum

        return res


def main(args=None):
    try:
        # NOTE: init context manager available starting from Kilted
        rclpy.init(args=args)
        service_relay = AsyncServiceRelay()

        rclpy.spin(service_relay)

    except (KeyboardInterrupt, ExternalShutdownException):
        pass
    finally:
        rclpy.try_shutdown()


if __name__ == '__main__':
    main()
