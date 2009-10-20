ifeq ($(origin SOLUTION_DIR), undefined)
	SOLUTION_DIR := $(shell pwd)/
endif
PROJECT_DIR_NDSECLIENT := $(dir $(lastword $(MAKEFILE_LIST)))

export NDSE_SDK:=$(SOLUTION_DIR)/../NDSE-SDK
export SOLUTION_DIR:=$(SOLUTION_DIR)

all:: NDSE_Client

include $(PROJECT_DIR_NDSECLIENT)/../KEYPAD-Actor/Unix/Makefile
include $(PROJECT_DIR_NDSECLIENT)/../VRAM-Reactor/Unix/Makefile
include $(PROJECT_DIR_NDSECLIENT)/../NDSE-Core/Unix/Makefile

clean::

NDSE_Client : libNDSE libKEYPAD_Actor libVRAM_Reactor 
	cd $(PROJECT_DIR_NDSECLIENT)/../Client && make
	cp $(PROJECT_DIR_NDSECLIENT)/../Client/NDSE_Client .
	cd $(PROJECT_DIR_NDSECLIENT)/../Client && make -i clean



