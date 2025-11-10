# Async Service Servers (Python)
> A demo package showcasing async service response possibilities

> [!CAUTION]
> Using async callbacks in `rclpy` are allowed in most places where callbacks are accepted.
> This allows for the use of `await` on [`rclpy.task.Future`](https://docs.ros.org/en/rolling/p/rclpy/rclpy.task.html#rclpy.task.Future), which then will return execution to the executor until work can be continued (such as handling an incoming service call response).
>
> The caviat is that **PARALLEL EXECUTION MUST BE ALLOWED ACCORDING TO THE CALLBACKGROUPS**, even when using the [`SingleThreadedExectutor`](https://docs.ros.org/en/rolling/p/rclpy/api/execution_and_callbacks.html#rclpy.executors.SingleThreadedExecutor).
> So callback group execution rules, will still apply.
> E.g. Everything in the same `MutuallyExclusiveCallbackGroup` cannot await on each other.

> [!WARNING]
> The section above is mostlikey still incomplete.

## Async Service Relay
When using normal service callback it is impossible to call another service from it when using the `SingleThreadedExecutor`, since the executor needs to handle the response, but it is being blocked by the service callback.

A solution to this would be an async service server callback.

An relay example has been made `async_service_relay`.
Which provides the a service `~/add_two_ints`, which relays the call to the `add_two_ints` service.

## Add Three Ints Server
This node calls `add_two_ints` twice (sequentially), to implement an `add_three_ints` service.
This would be impossible using a `sync` service handling callback and the `SingleThreadedExecutor`, since the executor needs to handle the response, but it is being blocked by the service callback.

A solution to this would be an async service server callback.
An add three ints server has been made (`add_three_ints_server`)
Which provides the a service `add_three_ints`, which delegates adding to a `add_two_ints` service.
