#!/bin/bash

if [ "$4" == "" ]
then
	echo "Usage: restart.sh <PRG> <SIZE> <FILENAME> <TIME>"
	exit 0
fi

PRG=$1
SIZE=$2
FILENAME=$3
TIME=$4

GO=1

while [ $GO -eq 1 ]
do
	./$PRG $SIZE 1 > $FILENAME &
        sleep 3
	ps aux
	echo "Programm gestartet, es sollte als Prozess angezeigt werden:"
	PID=$(ps aux | grep -e "$PRG" | awk '{print $1}')
	if [ "$PID" == "" ]; then
		echo "Programm bereits durchgelaufen, fertig!"
		exit 1
	else
	        echo "Programm gestartet mit PID $PID"
	fi
	sleep $TIME
	# kill $PID
	# sleep 5
	# ps aux
	ps aux | grep "$PRG" 
	if [ $? -eq 0 ]; then
		echo "Beende den Prozess $PID"
		kill $PID
		echo "Programm gestoppt mit PID $PID"
		
	        ps aux | grep "$PRG"
		while [ $? -eq 0 ]
		do
		    sleep 1
		    echo "Programm läuft noch, warte bis es beendet wurde ..."
	            ps aux | grep "$PRG"
		done
	else
		GO=0
	fi
done
