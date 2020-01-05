# bsp-stm32-f103
The board support package for all my stm32 f103 based projects. It is based on the STM32 f103rb nucleo eval board. 

The master is intended to provide only common things. If something specific to a derived PCB has to be implemented, this has to be done on a branch. Currently there are the following braches:

| Branch        | Description |
| ------------- |-------------|
| master        | For the stm32 nucleo f103 board. Generic functions only. |
| smartsink     | For the PCB used in the smart sink project. |
| | |