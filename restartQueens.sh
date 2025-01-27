#!/bin/bash


GO=1

while [ $GO -eq 1 ]
do
	./betterQueens.exe 10000 1 > queens-10000.txt &
	PID=$(ps aux | grep [b]etterQueens | awk '{print $1}')
	echo "Programm gestartet mit PID $PID"
	sleep 200
	# kill $PID
	# sleep 5
	ps aux
	ps aux | grep [b]etterQueens
	if [ $? -eq 0 ]; then
		kill $PID
		echo "Programm gestoppt mit PID $PID"
		
	        ps aux | grep [b]etterQueens
		while [ $? -eq 0 ]
		do
		    sleep 1
		    echo "Programm läuft noch, warte bis es beendet wurde ..."
	            ps aux | grep [b]etterQueens
		done
	else
		GO=0
	fi
done
