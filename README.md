# IOTOOL
### physical computing from userspace

*iotool* is a userspace application that provides access to multiple signal
inputs and outputs from a linux userspace. 

Currently the following types of resources are supported:
 - analog inputs, with configurable units and input gain
 - analog outputs (current and voltage mode)
 - current sources (sensor excitation tied to analog inputs)
 - digital inputs
 - general purpose digital outputs
 - bistable relays
 - leds
 - current monitors

The main interface to interace with iotool is a shell which can be run in
foreground or accessible over telnet. The main features of the sell are:
 - RPN context with
    - RPN stack
    - variables
    - constants
    - tree-like index of commands
    - online help


### Board abstraction model
iotool supports the following builds:
 - agp01, the AGP01 DAQ board, with corresponding yoctl layer located at
https://github.com/absobr/meta-agp01
 - emulator: a virtual board with no actual sensors, which used emulated
implementations of some resources to experiment with the shell features

### building
- checkout the sources and enter the iotool source directory and enter it
- create a build directory: `cd build-debug`
- initialize the build with: `cmake ..`
- run `make`

### Documentation
The `doc` directory contains some (limited) information about this tool and
shell usage.

In addition, the source code contains inline doxygen documentation for most
data types and methods.
