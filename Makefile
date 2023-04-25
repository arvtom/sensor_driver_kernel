# Disable forcing to declare variables at the beggining of function.
# It was enabled, because C90 is used by default.
ccflags-y := -Wno-declaration-after-statement
#ccflags-y += -std=gnu99

#what to build
obj-m := pcf8591_module.o

#add additional source files
pcf8591_module-objs := error_manager.o pcf8591.o

all:
	make -C /lib/modules/$(shell uname -r )/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r )/build M=$(PWD) clean
