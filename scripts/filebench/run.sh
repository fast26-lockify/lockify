#!/bin/bash
sudo bash -c "echo 0 > /proc/sys/kernel/randomize_va_space"
sudo bash -c "echo 3 > /proc/sys/vm/drop_caches"
filebench -f fileserver.f
filebench -f webproxy.f
