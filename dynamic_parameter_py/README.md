# Dynamic Parameter Types (Python)
> A demo package showcasing possible uses of parameters with dynamic types

> [!CAUTION]
> Using parameters with `dynamic_typing` enabled allows for these parameters to change type during execution, and allows for declaring yet-to-be-typed parameters.
> This also allows for these parameters to be deleted.
>
> There are several caveats with this:
> 1. The **CODE INTERACTING WITH THESE PARAMETERS MUST ACCEPT THESE TYPES**, which **POSSIBLY REQUIRES A CUSTOM `on_set_parameters` CALLBACK FOR (TYPE) VALIDATION**. Otherwise arbitrary types will be excepted, which could cause errors down the line.
> 2. **DYNAMICALLY TYPED PARAMETERS CAN BE DELETED *permanently*** when no special measures are taken.
> These special measures could be setting `allow_undeclared_parameters`, recreating the parameters on deletion or disallow parameter deletion manually.

> [!WARNING]
> The section above is most likely still incomplete.

## Dynamic Parameter Talker
By default, parameters are declared with a static type.
These types are strictly enforced, which causes values with a different type than the parameter to be rejected.

In order to allow a parameter to contain a value of an arbitrary type, `dynamic_typing` must be enabled.

A talker example has been made `dynamic_parameter_talker`.
Which publishes a `chatter` topic of type `example_interfaces/msg/String`, which contains the message number and the value/state of dynamically typed parameters.

<details>
<summary><b>Available Parameters</b></summary>
The following parameters:

#### message_number (integer)
The message number published in the message on the `chatter` topic.
This parameter gets updated to reflect the message count.

Default: `0`


#### my_first_dynamic_parameter (dynamic)
This parameter can change type during execution.

Default: `None`

Additional Constraints: This value should be interesting.

>[!NOTE]
> This parameter will be reset when it gets deleted.


#### dynamic_parameter_with_default (dynamic)
This parameter can change type during execution, but defaults to an integer.

Default: `12`

>[!NOTE]
> This parameter will be reset to `NOT_SET` when it gets deleted.

</details>


<details>
<summary><b>Running this example</b></summary>

After setting up and building a workspace (containing this package).
In a terminal (1) run the following:
```console
$ ros2 run dynamic_parameter_py dynamic_parameter_talker
```
This starts start the `dynamic_parameter_talker` node which published the `chatter` topic.

In order to view the `chatter` topic run the following in another terminal (2):
```console
$ # The command is setup to only print the data field of the message to ensure proper formatting
$ ros2 topic echo /chatter --field data
```

This will continuously output the data field of published messages on the `chatter` topic.
The first few messages will look something like this:
```
Message number: 10
'my_first_dynamic_parameter' is not set!
'dynamic_parameter_with_default' is type INTEGER and has value 12
---
Message number: 11
'my_first_dynamic_parameter' is not set!
'dynamic_parameter_with_default' is type INTEGER and has value 12
---
```

The parameter updates will be reflected on the `chatter` topic.

This can be done using the following commands (in another terminal (3)):
```console
$ # This will attempt to set the specified parameter to the value
$ ros2 param set /dynamic_parameter_talker <parameter_name> <parameter_value>

$ # This will attempt to delete the specified parameter
$ # This will reset the parameter if this is a dynamic parameter, however this is a custom behavior for this demo
$ ros2 param delete /dynamic_parameter_talker <parameter>
```

Using these commands will change the output of the command on terminal 2.

The log of the first terminal should look something like this:
```log
[INFO] [1774866405.632906683] [dynamic_parameter_talker]: Starting 'dynamic_parameter_talker'
[INFO] [1774866405.633406076] [dynamic_parameter_talker]: Parameter 'message_number' was updated.
[INFO] [1774866405.633740340] [dynamic_parameter_talker]: Parameter 'my_first_dynamic_parameter' was updated.
[INFO] [1774866405.634023737] [dynamic_parameter_talker]: 'my_first_dynamic_parameter' now has type NOT_SET
[INFO] [1774866405.634361999] [dynamic_parameter_talker]: Parameter 'dynamic_parameter_with_default' was updated.
[INFO] [1774866405.634646237] [dynamic_parameter_talker]: 'dynamic_parameter_with_default' now has type INTEGER
[INFO] [1774866406.639513756] [dynamic_parameter_talker]: Parameter 'message_number' was updated.
... # Skipped routine message updates
[INFO] [1774866429.638131656] [dynamic_parameter_talker]: Parameter 'message_number' was updated.
# Result of running: ros2 param set /dynamic_parameter_talker my_first_dynamic_parameter [1,2,3]
[INFO] [1774866430.290503337] [dynamic_parameter_talker]: Parameter 'my_first_dynamic_parameter' was updated.
[INFO] [1774866430.291303900] [dynamic_parameter_talker]: 'my_first_dynamic_parameter' now has type INTEGER_ARRAY
[INFO] [1774866430.639024548] [dynamic_parameter_talker]: Parameter 'message_number' was updated.
... # Skipped routine message updates
[INFO] [1774866437.638575941] [dynamic_parameter_talker]: Parameter 'message_number' was updated.
# Result of running: ros2 param set /dynamic_parameter_talker my_first_dynamic_parameter 4
[INFO] [1774866438.636705215] [dynamic_parameter_talker]: Parameter 'my_first_dynamic_parameter' was updated.
[INFO] [1774866438.637401767] [dynamic_parameter_talker]: 'my_first_dynamic_parameter' now has type INTEGER
[INFO] [1774866438.639056345] [dynamic_parameter_talker]: Parameter 'message_number' was updated.
... # Skipped routine message updates
[INFO] [1774866449.638147829] [dynamic_parameter_talker]: Parameter 'message_number' was updated.
# Result of running: ros2 param set /dynamic_parameter_talker dynamic_parameter_with_default 'kaas'
[INFO] [1774866450.451974012] [dynamic_parameter_talker]: Parameter 'dynamic_parameter_with_default' was updated.
[INFO] [1774866450.452702716] [dynamic_parameter_talker]: 'dynamic_parameter_with_default' now has type STRING
[INFO] [1774866450.638654074] [dynamic_parameter_talker]: Parameter 'message_number' was updated.
... # Skipped routine message updates
[INFO] [1774866465.638543124] [dynamic_parameter_talker]: Parameter 'message_number' was updated.
# Result of running: ros2 param delete /dynamic_parameter_talker my_first_dynamic_parameter
[INFO] [1774866466.448096649] [dynamic_parameter_talker]: Parameter 'my_first_dynamic_parameter' was updated.
[WARN] [1774866466.448942557] [dynamic_parameter_talker]: Parameter 'my_first_dynamic_parameter' has been recreated
[INFO] [1774866466.449925199] [dynamic_parameter_talker]: Parameter 'my_first_dynamic_parameter' was updated.
[INFO] [1774866466.450843286] [dynamic_parameter_talker]: 'my_first_dynamic_parameter' now has type NOT_SET
[INFO] [1774866466.638540853] [dynamic_parameter_talker]: Parameter 'message_number' was updated.
... # Skipped routine message updates
[INFO] [1774866479.638545269] [dynamic_parameter_talker]: Parameter 'message_number' was updated.
# Result of running: ros2 param delete /dynamic_parameter_talker dynamic_parameter_with_default
[INFO] [1774866480.050594100] [dynamic_parameter_talker]: Parameter 'dynamic_parameter_with_default' was updated.
[WARN] [1774866480.051202371] [dynamic_parameter_talker]: Parameter 'dynamic_parameter_with_default' has been recreated
[INFO] [1774866480.052024703] [dynamic_parameter_talker]: Parameter 'dynamic_parameter_with_default' was updated.
[INFO] [1774866480.052899436] [dynamic_parameter_talker]: 'dynamic_parameter_with_default' now has type INTEGER
[INFO] [1774866480.638704887] [dynamic_parameter_talker]: Parameter 'message_number' was updated.
```
</details>

## Lenient Multiplier
This node multiplies incoming numbers on the `number` topic with its `multiplier` parameter, the result of which gets published on the `multiplied_number` topic.
The `number` parameter is setup to be lenient by default, accepting both integer and float parameter values.
This behavior is achieved by making this parameter dynamically typed, while introducing type validation in a `on_set_parameters` callback.

This custom lenient behavior can be disabled by the setting the `is_lenient` parameter to `False` when the node starts.

This example also uses the `number_generator` node, which will increasing numbers automatically.

<details>
<summary><b>Running this example</b></summary>

After setting up and building a workspace (containing this package).
In a terminal (1) run the following:
```console
$ ros2 launch dynamic_parameter_py multiplier.launch.xml
```

>[!NOTE]
> This can be example can be run in different configurations, all options can be found by running:
> ```console
> $ ros2 launch dynamic_parameter_py multiplier.launch.xml -s
> ```
> A few notable options are:
> - The `lenient_multiplier` node can also be executed in strict mode (equivalent to using normal parameters), by setting `mode:=strict`.
> - The `number_generator` node can be disabled, by setting `start_number_generator:=False`.

In order to view the `multiplied_number` topic run the following in another terminal (2):
```console
$ ros2 topic echo /multiplied_number
```
The first few messages will look something like this:
```
data: 0.2
---
data: 0.30000000000000004
---
data: 0.4
---
data: 0.5
---
```

The `multiplier` parameter can be changed at runtime by using the following commands (in another terminal (3)):
```console
$ ros2 param set /lenient_multiplier multiplier <parameter_value>
```
The parameter value can be any numeric type (when not running this demo in strict mode).
This means it will accept:
 - Integers:
   - Decimal (pattern: `[+-]?\d[0-9_]*`, e.g. `+100_030` or `1234`)
   - Binary (pattern: `[+-]?0b_*[01][01_]*`, e.g. `0b0101_0010` or `0b100`)
   - Hexadecimal (pattern: `[+-]?0x_*[0-9a-fA-F][0-9a-fA-F_]*`, e.g. `0xFF_FF` or `0xF0E1`)
 - Floats:
   - Numbers (pattern: `([+-]\d[0-9_]*).\d[0-9_]([eE][+-]\d+)?`, e.g. `-21_000.3` or `.0e-4`)

> [!WARNING]
> Sometimes add `--` in front of the value is necessary for the arguments to parse correctly, often when containing `-`.
> This is the result of the argument parser trying to parse a *flag*, which is actually a value.
>
> For example
> ```console
> $ # Not working
> $ ros2 param set /lenient_multiplier multiplier -0xF
> usage: ros2 param set [-h] [--spin-time SPIN_TIME] [-s] [--no-daemon] [--include-hidden-nodes] [--timeout N] node_name parameter_name value
> ros2 param set: error: the following arguments are required: value
> $ ros2 param set /lenient_multiplier multiplier -- -0xF
> Set parameter successful
> ```

The log of the first terminal should look something like this:
```log
1774882608.3211935 [INFO] [launch]: All log files can be found below /home/user/.ros/log/2026-03-30-16-56-48-320356-Hostname-143351
1774882608.3213248 [INFO] [launch]: Default logging verbosity is set to INFO
1774882608.4757555 [INFO] [launch.user]: Starting the Number Generator from 0.0 @ 1.0 Hz
1774882608.4811797 [INFO] [number_generator-1]: process started with pid [143356]
1774882608.4815443 [INFO] [lenient_multiplier-2]: process started with pid [143357]
1774882608.8421278 [lenient_multiplier-2] [INFO] [1774882608.841928511] [lenient_multiplier]: The 'multiplier' parameter is lenient
1774882608.8426344 [lenient_multiplier-2] [INFO] [1774882608.842454022] [lenient_multiplier]: The multiplier has been set to 1.0
1774882609.8537946 [number_generator-1] [INFO] [1774882609.853545175] [number_generator]: Publishing: 0.0
1774882609.8554540 [lenient_multiplier-2] [INFO] [1774882609.855151264] [lenient_multiplier]: Multiplying 0.0 with multiplier 1.0
1774882610.8406117 [number_generator-1] [INFO] [1774882610.840372229] [number_generator]: Publishing: 0.1
1774882610.8421438 [lenient_multiplier-2] [INFO] [1774882610.841880832] [lenient_multiplier]: Multiplying 0.1 with multiplier 1.0
1774882611.8404920 [number_generator-1] [INFO] [1774882611.840142821] [number_generator]: Publishing: 0.2
1774882611.8419969 [lenient_multiplier-2] [INFO] [1774882611.841694456] [lenient_multiplier]: Multiplying 0.2 with multiplier 1.0
... Skipped
1774882618.8408773 [number_generator-1] [INFO] [1774882618.840610434] [number_generator]: Publishing: 0.8999999999999999
1774882618.8428252 [lenient_multiplier-2] [INFO] [1774882618.842444387] [lenient_multiplier]: Multiplying 0.8999999999999999 with multiplier 1.0
1774882619.8403058 [number_generator-1] [INFO] [1774882619.840131314] [number_generator]: Publishing: 0.9999999999999999
1774882619.8415825 [lenient_multiplier-2] [INFO] [1774882619.841334365] [lenient_multiplier]: Multiplying 0.9999999999999999 with multiplier 1.0
1774882620.3340690 [lenient_multiplier-2] [INFO] [1774882620.333770226] [lenient_multiplier]: The multiplier has been set to 10
1774882620.8408444 [number_generator-1] [INFO] [1774882620.840672417] [number_generator]: Publishing: 1.0999999999999999
1774882620.8428049 [lenient_multiplier-2] [INFO] [1774882620.842497793] [lenient_multiplier]: Multiplying 1.0999999999999999 with multiplier 10
1774882621.8408906 [number_generator-1] [INFO] [1774882621.840582817] [number_generator]: Publishing: 1.2
1774882621.8426025 [lenient_multiplier-2] [INFO] [1774882621.842285590] [lenient_multiplier]: Multiplying 1.2 with multiplier 10
... Skipped
1774882628.8409884 [number_generator-1] [INFO] [1774882628.840648281] [number_generator]: Publishing: 1.9000000000000006
1774882628.8424759 [lenient_multiplier-2] [INFO] [1774882628.842150121] [lenient_multiplier]: Multiplying 1.9000000000000006 with multiplier 10
1774882629.8403118 [number_generator-1] [INFO] [1774882629.840107153] [number_generator]: Publishing: 2.0000000000000004
1774882629.8418877 [lenient_multiplier-2] [INFO] [1774882629.841592742] [lenient_multiplier]: Multiplying 2.0000000000000004 with multiplier 10
1774882630.5417848 [lenient_multiplier-2] [INFO] [1774882630.541462016] [lenient_multiplier]: The multiplier has been set to -0.0001
1774882630.8406978 [number_generator-1] [INFO] [1774882630.840531344] [number_generator]: Publishing: 2.1000000000000005
1774882630.8424373 [lenient_multiplier-2] [INFO] [1774882630.842109419] [lenient_multiplier]: Multiplying 2.1000000000000005 with multiplier -0.0001
1774882631.8405027 [number_generator-1] [INFO] [1774882631.840327708] [number_generator]: Publishing: 2.2000000000000006
1774882631.8421178 [lenient_multiplier-2] [INFO] [1774882631.841814539] [lenient_multiplier]: Multiplying 2.2000000000000006 with multiplier -0.0001
... Skipped
1774882634.8412085 [number_generator-1] [INFO] [1774882634.840936848] [number_generator]: Publishing: 2.500000000000001
1774882634.8430927 [lenient_multiplier-2] [INFO] [1774882634.842786540] [lenient_multiplier]: Multiplying 2.500000000000001 with multiplier -0.0001
1774882635.1711428 [WARNING] [launch]: user interrupted with ctrl-c (SIGINT)
1774882635.3559122 [INFO] [number_generator-1]: process has finished cleanly [pid 143356]
1774882635.3567722 [INFO] [lenient_multiplier-2]: process has finished cleanly [pid 143357]
```
</details>
