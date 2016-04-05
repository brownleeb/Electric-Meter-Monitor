#!/bin/sh
/usr/bin/rtl_tcp -n 0 &
sleep 5
/root/go/bin/rtlamr -centerfreq=910000000 -quiet -filterid xxxxxxxxx -format=csv -symbollength=72 -decimation=18 -logfile=/root/rtlamr.out &
sleep 5
/usr/bin/php /root/sdr_power.php
sleep 5
pkill rtlamr
sleep 5
pkill -9 rtl_tcp
rm /root/rtlamr.out
