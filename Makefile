BINARY      = libbsp
COMPATIBLE  = stm32f103rb
DEPENDS     = libopencm3 libcli
SRCEXT      ?= c
DEFS        =
WARN_FLAGS  = -Wall
INCLUDE     = $(SRCDIR)/include
OPT         = -Og
EXPORT      = /include/bsp

###############################################################################
# Call generic library makefile
###############################################################################
include $(MYBUILDROOTPATH)/makefiles/library.mk
