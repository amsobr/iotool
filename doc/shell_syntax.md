# Help Modes

Possible invocations of help command
## help

This form asks for the most basic form of help.

Example reply

~~~
iotool version n.n.n-extra
Help commands:
    help              - display this message
    help providers    - list all registered shell providers

Provider-specific help:
    help-PREFIX1
    help-PREFIX2
    help-PREFIXN

List of commands:
	with ADCn cmd     - do whatever
	with ADCn cmd	  - do whatelse
	with DACn cmd     - do some more
	bucket cmd        - do something more
	


~~~

## help providers 

    Registered command prefixes:
        prefix1
        prefix2
        prefix3
        prefix4
        prefixN







device cmd [ARGS...]        <---- comandos genéricos via interface Type=ADC|DAC,etc
!system_cmd ARGS            <---- comandos de sistema - parser dedicado




!

