[TOC]



# Scheduler Configuration

A Scheduler automates the workflow of handling the hardware so
that data acquisition takes place.
The acquisition workflow contains three stages:
 1. Setup
 2. Execution
 3. Teardown

 Each stage is triggered by a command issued on the CMD shell:
  - session setup
  - session start
  - session teardown

## Setup phase
 This phase is intended to prepare the hardware for signal aquisition.
 Usually steps performed here deal with setting up the transducers,
 with parameters like SPS, full-scale, sensor excitations, etc.

## Execution phase
 Signal acquisition....

## Teardown phase
 Shutdown hardware resources, if relevant, in order to conserve
 CPU, power, reduce interference with external devices or whatever.

# Definition for a Scheduler
~~~json
{
  "schedulerType" : "periodic" ,
  "schedulerConfig" : {
    "setupScript" : [
      "with adc0 setopt sps=500" ,
      "with adc0 isource set source=0 ch=0 mag=50u" ,
      "with dac0 set ch=0 level=1000" ,
      "..."
    ] ,
    "executionInterval" : 100 ,
    "executionScript" : [
      "bucket init name=power" ,
      "with adc0 read ch=0" ,
      "with adc0 read ch=1" ,
      "bucket flush"
    ]  ,
    "teardownScript" : [
      "do" ,
      "whatever" ,
      "to" ,
      "idle" ,
      "hw"
    ]
  }
}
~~~

# RPN Library

## Filesystem structure
Resolver loads addons from directory:

## Function Definition

~~~JSON
{
  "functionName" : "ARG" ,
  "script" : [
    "instr1" ,
    "instr2" ,
    "instr3"  
  ]
}
~~~


# TransformJob Definition
~~~JSON
{
  "myJobName" : "ARG" ,
  "script" : [
    "instr1" ,
    "instr2" ,
    "instr3" 
  ] ,
  "outputs" : {
    "output_name1" : 0 ,
    "output_name2" : 1 ,
    "output_namex" : 2
  }
}
~~~

# Output Channel configuration
## Basic structure of an output myChannel

~~~json
{
	"myJobName" : "ARG" ,
	"myChannelName" : "CHANNEL_NAME" ,
	"channelType" : "CHANNEL_TYPE" ,
	"parameters" : {
		"param1" : "value1" ,
		"param2": "value2"
	}
}
~~~

The **parameters** portion is specific to the __channelType__.
The construction of output channels shall be performed by delegating the actual construction of the myChannel instances based on __channelType__

### CSV Writer
Following is the definition of a CSV writer

~~~json
{
	"myJobName" : "ARG" ,
	"myChannelName" : "CHANNEL_NAME" ,
	"channelType" : "csv_writer" ,
	"parameters"  :  {
		"fileName"  : "path/to/file.csv" ,
		"fieldSeparator" : "tab|colon|semicolon|space|comma" ,
		"append" : true|false ,
		"fields" : [
			"field_name_1" ,
			"field_name_2" ,
			"field_name_3" ,
			"field_name_n"			
		]
	}
}
~~~

# Structure of a Job Configuration

A job configurations consists of a directory with subdirectories for each stage of the automation engine. The needed subdirectories are:

| subdirectory      | role      |
| ---- | ---- |
| schedulers | Acquisition Schedulers. One JSON file per scheduler      |
| transform_jobs | Data transformation jobs. One JSON file per job      |
| output_channels | Output channel configurations. One JSON file per channel  |
| rpnlib_addons | User defined addons for the RPN lib (functions and constants). If this directory exists it will be loaded by the Resolver. Each addon is a JSON file. |


