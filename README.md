# Advanced ROS 2 Demos
> Advanced ROS 2 demos, exploring some lesser known features which are not suitable for beginners

> [!CAUTION]
> This repo is intended for **advanced** ROS2 users.
> It explains some undocumented functionalities, however some of these are **undocumented for a good reason**.
> Often because of the included caveats and footguns.

Each ROS 2 package consists of its own self-contained demonstration(s) with its respective `README.md` showing how things work.

In short:
 - [`deferred_service_response_cpp`](deferred_service_response_cpp/README.md): `rclcpp` allows to defer sending of the service response to a later moment, allowing the executor to continue.
 - [`async_service_py`](async_service_py/README.md): `rclpy` allows to async callbacks in service servers (and most other callbacks) to enable await, allowing the executor to continue.
 - [`dynamic_parameter_py`](dynamic_parameter_py/README.md): Using dynamically-typed parameters with `rclpy`, which allows for the type or parameters to change at runtime.

# Developing
This repository uses pre-commit.
To set this up locally, make sure pre-commit is installed and run the following command
```shell
  pre-commit install
```
