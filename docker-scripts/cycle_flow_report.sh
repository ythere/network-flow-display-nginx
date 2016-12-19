#!/bin/bash
cycle_report="/vbox/bin/flow_report.sh"
for ((i=0;i<12;i++));
do
	$cycle_report
	sleep 5
done
echo "success"
	
