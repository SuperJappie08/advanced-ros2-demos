# Deferred Service Response (C++)
> A demo package showcasing deferred service response possibilities

> [!CAUTION]
> Using deferred service responses allows delaying a service response, which allows the callback to be shorter.
> This also allows for other work to be processed by the executor in the meantime, (such as handling an incoming service call response).
>
> The caveat is that **YOU ARE RESPONSIBLE FOR RESPONDING**.
> When not responding can lead to hanging or memory-leaking clients.
> A client will hang if it is explicitly spinning until the response future is complete.
> When a client does not wait for the response, it will allocate memory for the response as documented in [`rclcpp::Client::async_send_request`](https://docs.ros.org/en/rolling/p/rclcpp/generated/classrclcpp_1_1Client.html#_CPPv4N6rclcpp6Client18async_send_requestE13SharedRequest)

> [!WARNING]
> The section above is incomplete, in short: A deferred service can also be executes multiple times simultaneously.

## Async Service Relay
When using normal service callback it is impossible to call another service from it when using the `SingleThreadedExecutor`, since the executor needs to handle the response, but it is being blocked by the service callback.

A solution to this would be a deferred service response.

A relay example has been made <span title="Available as component and node">`async_service_relay`</span>.
Which provides a service `~/add_two_ints`, which relays the call to the `add_two_ints` service.

<details>
<summary><b>Running this example (Normal)</b></summary>

After setting up and building a workspace (containing this package).
In a terminal 1 run the following:
```console
$ ros2 launch deferred_service_response_cpp async_service_relay.launch.xml
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
[INFO] [launch]: All log files can be found below /home/user/.ros/log/2025-11-10-20-32-46-667735-J001-29771
[INFO] [launch]: Default logging verbosity is set to INFO
[INFO] [add_two_ints_server-1]: process started with pid [29774]
[INFO] [async_service_relay-2]: process started with pid [29775]
[async_service_relay-2] [INFO] [1762803166.861475760] [async_service_relay]: Starting the service relay
[async_service_relay-2] [INFO] [1762803284.563571360] [async_service_relay]: Deferring service callback 1
[add_two_ints_server-1] [INFO] [1762803284.564002820] [add_two_ints_server]: Incoming request
[add_two_ints_server-1] a: 10 b: 30
[async_service_relay-2] [INFO] [1762803284.564528600] [async_service_relay]: Response received for 1
```
</details>

<details>
<summary><b>Running this example (Composition)</b></summary>

After setting up and building a workspace (containing this package).
In a terminal 1 run the following:
```console
$ ros2 launch deferred_service_response_cpp async_service_relay_composition.launch.xml
```
This will start the `async_service_relay` and a `add_two_ints_server` in a single threaded component container (`add_ints_container`).
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
[INFO] [launch]: All log files can be found below /home/user/.ros/log/2025-11-10-20-38-04-909857-J001-31008
[INFO] [launch]: Default logging verbosity is set to INFO
[INFO] [component_container-1]: process started with pid [31031]
[component_container-1] [INFO] [1762803485.628804590] [add_ints_container]: Load Library: /opt/ros/jazzy/lib/libserver_component.so
[component_container-1] [INFO] [1762803485.634658100] [add_ints_container]: Found class: rclcpp_components::NodeFactoryTemplate<composition::Server>
[component_container-1] [INFO] [1762803485.634730190] [add_ints_container]: Instantiate class: rclcpp_components::NodeFactoryTemplate<composition::Server>
[INFO] [launch_ros.actions.load_composable_nodes]: Loaded node '/add_two_ints_server' in container '/add_ints_container'
[component_container-1] [INFO] [1762803485.948279570] [add_ints_container]: Load Library: /home/user/adv_demo_ws/install/deferred_service_response_cpp/lib/libasync_service_relay_component.so
[component_container-1] [INFO] [1762803485.949781130] [add_ints_container]: Found class: rclcpp_components::NodeFactoryTemplate<deferred_service_response_cpp::AsyncServiceRelay>
[component_container-1] [INFO] [1762803485.949829820] [add_ints_container]: Instantiate class: rclcpp_components::NodeFactoryTemplate<deferred_service_response_cpp::AsyncServiceRelay>
[component_container-1] [INFO] [1762803485.956315940] [async_service_relay]: Starting the service relay
[INFO] [launch_ros.actions.load_composable_nodes]: Loaded node '/async_service_relay' in container '/add_ints_container'
[component_container-1] [INFO] [1762803496.894489700] [async_service_relay]: Deferring service callback 1
[component_container-1] [INFO] [1762803496.895127980] [add_two_ints_server]: Incoming request: [a: 10, b: 30]
[component_container-1] [INFO] [1762803496.895370530] [async_service_relay]: Response received for 1
```
</details>

## Add Three Ints Server
This node calls `add_two_ints` twice (sequentially), to implement an `add_three_ints` service.
This would be impossible using a `sync` service handling callback and the `SingleThreadedExecutor`, since the executor needs to handle the response, but it is being blocked by the service callback.

A solution to this would be an async service server callback.
An add three ints server has been made (<span title="Available as component and node">`add_three_ints_server`</span>.)
Which provides the `add_three_ints` service, which delegates adding to a `add_two_ints` service.

<details>
<summary><b>Running this example (Normal)</b></summary>

After setting up and building a workspace (containing this package).
In a terminal 1 run the following:
```console
$ ros2 launch deferred_service_response_cpp add_three_ints.launch.xml
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
[INFO] [launch]: All log files can be found below /home/user/.ros/log/2025-11-10-20-48-08-663783-J001-34150
[INFO] [launch]: Default logging verbosity is set to INFO
[INFO] [add_two_ints_server-1]: process started with pid [34153]
[INFO] [add_three_ints_server-2]: process started with pid [34154]
[add_three_ints_server-2] [INFO] [1762804088.934740212] [add_three_ints_server]: Starting the add three ints server
[add_three_ints_server-2] [INFO] [1762804162.483970461] [add_three_ints_server]: Processing request 1: { a: 10, b: 30, c: 60 }
[add_two_ints_server-1] [INFO] [1762804162.484533231] [add_two_ints_server]: Incoming request
[add_two_ints_server-1] a: 10 b: 30
[add_three_ints_server-2] [INFO] [1762804162.485009421] [add_three_ints_server]: Received 'a + b = 40'
[add_two_ints_server-1] [INFO] [1762804162.485265471] [add_two_ints_server]: Incoming request
[add_two_ints_server-1] a: 40 b: 60
[add_three_ints_server-2] [INFO] [1762804162.485499741] [add_three_ints_server]: Received '(a + b) + c = 100'
```
</details>

<details>
<summary><b>Running this example (Composition)</b></summary>

After setting up and building a workspace (containing this package).
In a terminal 1 run the following:
```console
$ ros2 launch deferred_service_response_cpp add_three_ints_composition.launch.xml
```
This will start the `add_three_ints_server` and a `add_two_ints_server` in a single threaded component container (`add_ints_container`).
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
[INFO] [launch]: All log files can be found below /home/user/.ros/log/2025-11-10-20-51-42-664932-J001-34987
[INFO] [launch]: Default logging verbosity is set to INFO
[INFO] [component_container-1]: process started with pid [35004]
[component_container-1] [INFO] [1762804303.217727965] [add_ints_container]: Load Library: /opt/ros/jazzy/lib/libserver_component.so
[component_container-1] [INFO] [1762804303.219741237] [add_ints_container]: Found class: rclcpp_components::NodeFactoryTemplate<composition::Server>
[component_container-1] [INFO] [1762804303.219808923] [add_ints_container]: Instantiate class: rclcpp_components::NodeFactoryTemplate<composition::Server>
[INFO] [launch_ros.actions.load_composable_nodes]: Loaded node '/add_two_ints_server' in container '/add_ints_container'
[component_container-1] [INFO] [1762804303.230181136] [add_ints_container]: Load Library: /home/user/adv_demo_ws/install/deferred_service_response_cpp/lib/libadd_three_ints_server_component.so
[component_container-1] [INFO] [1762804303.231064663] [add_ints_container]: Found class: rclcpp_components::NodeFactoryTemplate<deferred_service_response_cpp::AddThreeIntsServer>
[component_container-1] [INFO] [1762804303.231119499] [add_ints_container]: Instantiate class: rclcpp_components::NodeFactoryTemplate<deferred_service_response_cpp::AddThreeIntsServer>
[component_container-1] [INFO] [1762804303.237825400] [add_three_ints_server]: Starting the add three ints server
[INFO] [launch_ros.actions.load_composable_nodes]: Loaded node '/add_three_ints_server' in container '/add_ints_container'
[component_container-1] [INFO] [1762804307.924448401] [add_three_ints_server]: Processing request 1: { a: 10, b: 30, c: 60 }
[component_container-1] [INFO] [1762804307.925137643] [add_two_ints_server]: Incoming request: [a: 10, b: 30]
[component_container-1] [INFO] [1762804307.925404680] [add_three_ints_server]: Received 'a + b = 40'
[component_container-1] [INFO] [1762804307.925467272] [add_two_ints_server]: Incoming request: [a: 40, b: 60]
[component_container-1] [INFO] [1762804307.925492998] [add_three_ints_server]: Received '(a + b) + c = 100'
```
</details>
