#!/bin/bash
# sectraffic_flow_count.sh Tx Rx

Vpn_data_file="/vbox/config/mintraffic_vpn_data.txt"
Vpn_data_file_result="/vbox/config/mintraffic_vpn_data_result.txt"

Tx_data_before=`cat $Vpn_data_file|awk -F ' ' {'print $1'}`
Rx_data_before=`cat $Vpn_data_file|awk -F ' ' {'print $2'}`

echo $1 $2 >$Vpn_data_file

Tx_data_total=$[$1-Tx_data_before]
Rx_data_total=$[$2-Rx_data_before]
Time=`date +%s`

echo $Tx_data_total $Rx_data_total $Time >$Vpn_data_file_result





