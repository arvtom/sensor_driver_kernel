# MODULE_NAME = pcf8591
# SRC_FILES = pcf8591.c
# # error_manager.c
# KERNEL_HEADERS = /lib/modules/$(shell uname -r)/build/include
# CCFLAGS = -I$(KERNEL_HEADERS)
# $(MODULE_NAME).ko: $(SRC_FILES)
# all:
# 	make -C /lib/modules/$(shell uname -r )/build M=$(PWD) modules

# clean:
# 	make -C /lib/modules/$(shell uname -r )/build M=$(PWD) clean

ccflags-y := -std=gnu99 -Wno-declaration-after-statement
obj-m := pcf8591.o
#pcf8591-y := $(src)/pcf8591.o
#$(src)/pcf8591.o := $(src)/pcf8591.h
#pcf8591-objs := error_manager.o pcf8591.o
INC=-I/usr/include
PWD:=$(shell pwd)
all:
	make -C /lib/modules/$(shell uname -r )/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r )/build M=$(PWD) clean
