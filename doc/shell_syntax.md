# iotool shell
The iotool shell combines a tree-like structure that holds available
peripherals, grouped by peripheral type, and a Reverse-Polish-Notation (RPN)
stack context.

## Peripherals tree
The peripherals are organized in a tree, similar to a unix filesystem.
The first ADC on the board is located at `/adc/0`, the second ADC is located
at `/adc/1` and so forth.

## RPN context
The RPN context is used by peripheral commands to read and write values from
input/output operations. For instance, a power monitor status command will push
two values to the stack, which can then be used to perform further calculations.

```text
/pm/0> state
01: 0.100638
00: 12.144000
/pm/0>
```
In the example above, while located at `/pm/0`, the user ran the command `state`
The command state read the current consumption and supply voltage and pushed
them to the stack. When the command completes, the shell shows the new values
added to the stack.

*NOTE: There is no visual indication when commands pop values from the stack.
To inspect the contents of the stack at any point, use the `show stack`
command*

## Operation of the interactive shell
The interactive shell is accessed by runnint `iotool --foreground` on the
console, or by establishing a telnet connection to port **TBD** when _iotool_
is invoked as `iotool --TBD`

Each shell has its own context. The interactive shall supports multiple
concurrent connections. However, the physical resources of the board are
equally available on all shell instances and it is the user's responsibility to
ensure the consistent handling of the physical resources.

The interactive shell contains:
* *working path*: selects a peripheral and related commands
* *RPN stack*
* *Named Variables*: Store values from the stack with user-defined names and
  allow pushing those values back into the stack at any time using variable
  handling commands.
* *Constants*: Offer the same functionality as variables, with the exception
  that once a constant is created, its value can't be modified.

### Command scope
Commands are organized in a tree. The position of a command within the tree
determines its scope. A command defined in a given path, is available on that
path and all its sub-paths.

The iotool shell supports commands with the same name on different paths. The
actual command executed is determined by the working path at the time of the
invocation of the command.

#### Command lookup
When a command is invoked, the shell looks up the command starting on the
current working path. 
* If the command is found, lookup is complete and the command is executed
* If the command is not found, lookup the command on the parent path.
* The lookup fails with error is the command is not found on the root path.

It is possible to have different commands with the same name on different paths.
The lookup algorithm ensures that the appropriate command is executed if the
user changes the working directory correctly before invoking the command.

The example below shows how different commands with the same name can be made
available:
```text
/
/dout                 <-- digital outputs
    [cmd:status]
    [cmd:set]
/dout/0               <-- relays
/indicator            <-- signaling devices (leds, etc)
    [cmd:status]
    [cmd:set]
/indicator/0          <-- board LEDs
```
* Invoking `status` from _/indicator/0_ will run the command available on the
  location _/indicator_
* Invoking `status` from _/dout/0_ will run the command available on the
  location _/dout_
* Invoking `status` from the root will fail because the command does not exist
  in that scope.

#### Peripheral lookup
A board contains a number of peripherals which are organized in a tree like 
structure. The current working path serves the double purpose of exposing the
available commands, as well as to select the peripheral on which some of those
commands will operate on.

Example:
```text
/
/adc
  [cmd:read]
  [cmd:isource]
/adc/0              <-- first ADC peripheral
/adc/1              <-- second ADC peripheral
```
To execute the _read_ command on the first ADC, the user must first change the
working path to `/adc/0` and then invoke `read ARGS...`
To execte the _read_ command on the second ADC, the user must first change the
working path to `/adc/1`and then invoke `read ARGS...`

In both cases, the executed read command will be the same, but on each invocation
a different ADC will be selected due to the working path.

Attempting to run the `read` command from the `/adc` path will fail because
no peripheral exists at that location.

## Online help
To obtain help on the shell, use the following commands:
* *help* shows available commands and peripherals on the current working path
* *show* shows information about the current shell context, like variables, 
  constants, the stack and available peripherals

### help command

### General help

```text
/> cd /dout
/dout>
/dout> help
Currently on path: /dout
Available commands:
         set - Set one ore more relays
      status - Show current RELAY status

Available paths:
/dout/0
/dout/1
```
In this example we see the typical output of a help command:
* the working path (selects the peripheral on which commands will run)
* The list of commands available, which depends on the current working path
* The available paths below the current path.

### Help for a specific command

The help command may also be used to show online help about any available
command, as shown below:
```text
/> cd /adc
/adc >
/adc >help read
read: read ADC inputs
Usage:
read ch=ARG           : read single ended
read chp=ARG chn=ARG  : read differential

Arguments:
ch        ID of the ADC channel: 0 .. n
chp       ID of the positive side input
chn       ID of the negative side input
```

