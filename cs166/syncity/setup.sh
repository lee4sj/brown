#!/bin/sh
WHOAMI=`whoami`
if [ -w guilenny.vmxe ]; then mv guilenny.vmx~ guilenny.vmx; fi
sed -i'~' -e "s/@@USERNAME@@/$WHOAMI/" guilenny.vmx
if [[ -z `cat ~/.vmware/config | grep "tmpDirectory"` ]]; then echo "tmpDirectory = /ltmp" >> ~/.vmware/config; fi
