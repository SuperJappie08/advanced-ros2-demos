# Async Service Servers (Python)
> A demo package showcasing async service response possibilities

> [!CAUTION]
> Using async callbacks in `rclpy` are allowed in most places where callbacks are accepted.
> This allows for the use of `await` on [`rclpy.task.Future`](https://docs.ros.org/en/rolling/p/rclpy/rclpy.task.html#rclpy.task.Future), which then will return execution to the executor until work can be continued (such as handling an incoming service call response).
>
> The caveat is that **PARALLEL EXECUTION MUST BE ALLOWED ACCORDING TO THE CALLBACKGROUPS**, even when using the [`SingleThreadedExectutor`](https://docs.ros.org/en/rolling/p/rclpy/api/execution_and_callbacks.html#rclpy.executors.SingleThreadedExecutor).
> So callback group execution rules, will still apply.
> E.g. Everything in the same `MutuallyExclusiveCallbackGroup` cannot await on each other.

> [!WARNING]
> The section above is most likely still incomplete.

## Async Service Relay
When using normal service callback it is impossible to call another service from it when using the `SingleThreadedExecutor`, since the executor needs to handle the response, but it is being blocked by the service callback.

A solution to this would be an async service server callback.

A relay example has been made `async_service_relay`.
Which provides a service `~/add_two_ints`, which relays the call to the `add_two_ints` service.

<details>
<summary><b>Running this example</b></summary>

After setting up and building a workspace (containing this package).
In a terminal 1 run the following:
```console
$ ros2 launch async_service_py async_service_relay.launch.xml
```
This will start the `async_service_relay` and a `add_two_ints_server`.
Now run the following in another terminal (2) (example output provided):
```console
$ ros2 service call /async_service_relay/add_two_ints example_interfaces/srv/AddTwoInts "{a: 10, b: 30}"
waiting for service to become available...
requester: making request: example_interfaces.srv.AddTwoInts_Request(a=10, b=30)

response:
example_interfaces.srv.AddTwoInts_Response(sum=40)
```
Terminal 1 now has approximately the following output:
```log
[INFO] [launch]: All log files can be found below /home/user/.ros/log/2025-11-10-20-59-20-315635-J001-36916
[INFO] [launch]: Default logging verbosity is set to INFO
[INFO] [add_two_ints_server-1]: process started with pid [36919]
[INFO] [async_service_relay-2]: process started with pid [36920]
[async_service_relay-2] [INFO] [1762804858.003871079] [async_service_relay]: Request received: example_interfaces.srv.AddTwoInts_Request(a=10, b=30)
[add_two_ints_server-1] [INFO] [1762804858.004484159] [add_two_ints_server]: Incoming request
[add_two_ints_server-1] a: 10 b: 30
[async_service_relay-2] [INFO] [1762804858.005443649] [async_service_relay]: Response received for example_interfaces.srv.AddTwoInts_Request(a=10, b=30)
```
</details>

## Add Three Ints Server
This node calls `add_two_ints` twice (sequentially), to implement an `add_three_ints` service.
This would be impossible using a `sync` service handling callback and the `SingleThreadedExecutor`, since the executor needs to handle the response, but it is being blocked by the service callback.

A solution to this would be an async service server callback.
An add three ints server has been made (`add_three_ints_server`)
Which provides the `add_three_ints` service, which delegates adding to a `add_two_ints` service.

<details>
<summary><b>Running this example</b></summary>

After setting up and building a workspace (containing this package).
In a terminal 1 run the following:
```console
$ ros2 launch async_service_py add_three_ints.launch.xml
```
This will start the `add_three_ints_server` and a `add_two_ints_server`.
Now run the following in another terminal (2) (example output provided):
```console
$ ros2 service call /add_three_ints advanced_demo_interfaces/srv/AddThreeInts "{a: 10, b: 30, c: 60}"
waiting for service to become available...
requester: making request: advanced_demo_interfaces.srv.AddThreeInts_Request(a=10, b=30, c=60)

response:
advanced_demo_interfaces.srv.AddThreeInts_Response(sum=100)
```
Terminal 1 now has approximately the following output:
```log
[INFO] [launch]: All log files can be found below /home/user/.ros/log/2025-11-10-21-06-45-360026-J001-38711
[INFO] [launch]: Default logging verbosity is set to INFO
[INFO] [add_two_ints_server-1]: process started with pid [38714]
[INFO] [add_three_ints_server-2]: process started with pid [38715]
[add_three_ints_server-2] [INFO] [1762805208.559125553] [add_three_ints_server]: Processing request: { a: 10, b: 30, c: 60 }
[add_two_ints_server-1] [INFO] [1762805208.559954633] [add_two_ints_server]: Incoming request
[add_two_ints_server-1] a: 10 b: 30
[add_three_ints_server-2] [INFO] [1762805208.561404623] [add_three_ints_server]: Received 'a + b = 40'
[add_two_ints_server-1] [INFO] [1762805208.561742213] [add_two_ints_server]: Incoming request
[add_two_ints_server-1] a: 40 b: 60
[add_three_ints_server-2] [INFO] [1762805208.562587673] [add_three_ints_server]: Received '(a + b) + c = 100'
```
</details>
