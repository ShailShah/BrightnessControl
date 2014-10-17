please issue all the commands as root

1) mknod /dev/bright c 119 1
2) chmod a+r+w /dev/bright
3) Goto Brightness folder
4) make
5) insmod bright.ko
6) cat /proc/modules  check if bright module is loaded
7) cat /proc/devices check if devie is available
8) gcc simulate.c -o simulate
9) ./simulate "set 6"