# Deferred Service Response (C++)
> A demo package showcasing deferred service response possibilities

> [!CAUTION]
> Using deferred service responses allows delaying a service response, which allows the callback to be shorter.
> This also allows for other work to be processed in the mean time by the executor, (such as handling an incoming service call response).
>
> The caviat is that **YOU ARE RESPONSIBLE FOR RESPONDING**.
> When not responding can lead to hanging or memory-leaking clients.
> A client will hang if it is explicitly spins until the response future is complete.
> When a client does not wait for the response, it will allocate memory for the response as documented in [`rclcpp::Client::async_send_request`](https://docs.ros.org/en/rolling/p/rclcpp/generated/classrclcpp_1_1Client.html#_CPPv4N6rclcpp6Client18async_send_requestE13SharedRequest)

> [!WARNING]
> The section above is incomplete, in short: A deferred service can also be executes multiple times simultaneously.

## Async Service Relay
When using normal service callback it is impossible to call another service from it when using the `SingleThreadedExecutor`, since the executor needs to handle the response, but it is being blocked by the service callback.

A solution to this would be a deferred service response.

An relay example has been made <span title="Available as component and node (async_service_relay_node)">`async_service_relay`</span>.
Which provides the a service `~/add_two_ints`, which relays the call to the `add_two_ints` service.
