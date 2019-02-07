#!/bin/bash

cd /usr/src/linux-2.4.18-14custom
make bzImage
if [ $? -eq 0 ] # if make returned 0, i.e. successfully made bzImage
then
	tar -czf kernel.tar.gz kernel/sched.c kernel/fork.c kernel/hw2_syscalls.c kernel/Makefile arch/i386/kernel/entry.S include/linux/sched.h
	echo "tarball creation completed"
else
	echo "tarball creation failed."
fi
