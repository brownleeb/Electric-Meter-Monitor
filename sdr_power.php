<?php
sleep(60);
exec('tail -1 /root/rtlamr.out',$result);
if (!empty(str_getcsv(implode($result))[7])) {
   $prev = str_getcsv(implode($result))[7];  }
$curr = $prev;
while ( $prev == $curr) {
   sleep(1);
   $prev = $curr;
   unset($result);
   exec('tail -1 /root/rtlamr.out',$result);
#   echo implode($result)."\n";
   if (!empty(str_getcsv(implode($result))[7])) {
      $curr = str_getcsv(implode($result))[7];  }
#   echo $prev . "   " . $curr . "\n";
}
exec("pkill -SIGHUP -f \"/usr/local/bin/power -g 13\"");
sleep(2);
$old = file_get_contents("/user1/power/power13.txt");
$curr = $curr + 100000;
file_put_contents("/user1/power/power13.txt", $curr."000");
exec("pkill -SIGUSR1 -f \"/usr/local/bin/power -g 13\"");
#exec("pkill -SIGTERM -x rtl_tpc");
$msg = "logger \"Wrote new house power: New = ".$curr."000, Old = ".$old."\"";
exec($msg);
$delta = ($curr * 1000) - $old;
$msg = "logger \"House Power delta: ".$delta."\"";
exec($msg);
?>
