#!/bin/bash

cd /usr/src/linux-2.4.18-14custom
make bzImage
if [ $? -eq 0 ] # if make returned 0, i.e. successfully made bzImage
then
	cd arch/i386/boot
	cp -f bzImage /boot/vmlinuz-2.4.18-14custom
	echo "compilation succeeded. rebooting..."
	reboot
else
	echo "compilation failed."
fi