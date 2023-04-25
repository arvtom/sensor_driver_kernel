ccflags-y := -std=gnu99 -Wno-declaration-after-statement
obj-m += pcf8591.o
#pcf8591-y := $(src)/pcf8591.o
#$(src)/pcf8591.o := $(src)/pcf8591.h
# pcf8591-objs+= error_manager.o
INC=-I/usr/include

all:
	make -C /lib/modules/$(shell uname -r )/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r )/build M=$(PWD) clean
