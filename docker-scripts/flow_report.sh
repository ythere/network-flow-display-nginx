#!/bin/bash

#默认文件中存0
flow_times_config=/vbox/config/flow_times_config

Counts=`cat $flow_times_config`
#sar命令来实现实时检测
interfacename=tap0
url="http://localhost/traffic"

#网卡总的收发量
Txtotal=`cat /proc/net/dev |grep $interfacename|awk -F ':' {'print $2'}|awk {'print $9'}`
Rxtotal=`cat /proc/net/dev |grep $interfacename|awk -F ':' {'print $2'}|awk {'print $1'}`

#组成json格式的数据并传输
Time=`date +%s`
json_traffic=`echo $json_traffic | sed 's/$/&{"Time": \"'"$Time"'\","Txtotal": \"'"$Txtotal"'\","Rxtotal": \"'"$Rxtotal"'\","Counts": \"'"$Counts"'\"}/g'`
echo $json_traffic
curl -d "$json_traffic" $url
echo $? >> /vbox/config/traffic_record

let Counts=Counts+1
echo $Counts >$flow_times_config





