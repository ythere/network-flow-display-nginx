#!/bin/bash 
# traffic_flow_count.sh  1 s
Gre_switch_file="/vbox/config/vpn/gre/main_gre_status.txt"
Interface_sec_data_file="/vbox/config/secinterface_$1.txt"
Interface_min_data_file="/vbox/config/mininterface_$1.txt"

Vpn_sec_data_file="/vbox/config/sectraffic_vpn_data_result.txt"
Vpn_min_data_file="/vbox/config/mintraffic_vpn_data_result.txt"

gre_switch_mode=`cat $Gre_switch_file`

function get_vpn_sec_data()
{
	cat $Vpn_sec_data_file
	return 0
}

function get_vpn_min_data()
{
	cat $Vpn_min_data_file
	return 0
}

function get_sec_data()
{
	Tx_data_before=`cat $Interface_sec_data_file|awk -F ' ' {'print $1'}`
	RX_data_before=`cat $Interface_sec_data_file|awk -F ' ' {'print $2'}`

	Tx_data_now=`cat /proc/net/dev |grep $interfacename |awk -F ' ' {'print $10'}`
	Rx_data_now=`cat /proc/net/dev |grep $interfacename |awk -F ' ' {'print $2'}`
	Time=`date +%s`

	Tx_data_total=$[Tx_data_now-Tx_data_before]
	Rx_data_total=$[Rx_data_now-RX_data_before]
	echo $Tx_data_now $Rx_data_now >$Interface_sec_data_file
	echo $Tx_data_total $Rx_data_total $Time
}
function get_min_data()
{
	Tx_data_before=`cat $Interface_min_data_file|awk -F ' ' {'print $1'}`
	RX_data_before=`cat $Interface_min_data_file|awk -F ' ' {'print $2'}`

	Tx_data_now=`cat /proc/net/dev |grep $interfacename |awk -F ' ' {'print $10'}`
	Rx_data_now=`cat /proc/net/dev |grep $interfacename |awk -F ' ' {'print $2'}`
	Time=`date +%s`

	Tx_data_total=$[Tx_data_now-Tx_data_before]
	Rx_data_total=$[Rx_data_now-RX_data_before]
	echo $Tx_data_now $Rx_data_now >$Interface_min_data_file
	echo $Tx_data_total $Rx_data_total $Time
}


if [ $1 -eq 6 ];then
	if [ $gre_switch_mode -eq 0 ]; then
		interfacename='gre'
	else
		interfacename='gre:0'
	fi
else
	if [ $1 -eq 5 ]; then 
		#GET VPN DATA AND ECHO
		if [ $2 == 's' ]; then
			get_vpn_sec_data
		else
			get_vpn_min_data
		fi
		exit 0 
	else
		interfacename=eth$1
	fi
fi
if [ $2 == 's' ];then
	get_sec_data $interfacename
else
	get_min_data $interfacename
fi
exit 0	




	
	
