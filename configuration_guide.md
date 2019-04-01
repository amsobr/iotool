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
~~~JSON
{
  "schedType" : "periodic" ,
  "setup" : {
    "script" : [
      "with adc0 setopt sps=500" ,
      "with adc0 isource set source=0 ch=0 mag=50u" ,
      "with dac0 set ch=0 level=1000" ,
      "..."
    ]
  },
  "execution" : {
    "executionInterval" : 100 ,
    "script" : [
      "bucket init name=power" ,
      "with adc0 read ch=0" ,
      "with adc0 read ch=1" ,
      "bucket flush"
    ]  
  },
  "teardown" : {
    "script" : [
      "do" ,
      "whatever" ,
      "to" ,
      "idle" ,
      "hw"
    ]
  }
}
~~~