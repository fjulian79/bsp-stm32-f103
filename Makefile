BINARY      = libbsp
COMPATIBLE  = stm32f103rb
DEPENDS     = libopencm3
SRCEXT      ?= c
DEFS        =
WARN_FLAGS  = -Wall
INCLUDE     = /include
OPT         = -Og
EXPORT      = /include/bsp

###############################################################################
# Call generic library makefile
###############################################################################
include $(MYBUILDROOTPATH)/makefiles/library.mk
