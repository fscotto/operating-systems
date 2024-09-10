obj-m += task-driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

sign:
	/usr/src/kernels/$(shell uname -r)/scripts/sign-file sha256 kernel-sign/private_key.priv kernel-sign/public_key.der task-driver.ko

