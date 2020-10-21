# router.spec
#
strip on

mkdir /dev
mknod /dev/console c 5 1
mknod /dev/ttyS0 c 4 64
mknod /dev/ttyS1 c 4 65

mkdir    /lib
mkdir -p /usr/lib
mkdir    /bin
mkdir    /sbin
mkdir -p /etc/rc.d
mkdir    /proc

cp ./local/fstab ./local/inittab  /etc
cp ./local/rc.sysinit             /etc/rc.d
cp ./router /sbin

lcd ${BLUECAT_PREFIX}/bin
cp ping mount bash /bin
ln -s /bin/bash /bin/sh

lcd ${BLUECAT_PREFIX}/sbin
cp reboot init ifconfig route  /sbin

chmod 755 /bin /sbin /sbin/router
# End of File
