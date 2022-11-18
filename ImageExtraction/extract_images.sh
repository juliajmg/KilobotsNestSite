#!/bin/bash

echo "filename: $1"
FOLDER=${1%.mp4*}
mkdir $FOLDER
echo "deltat: $2"
echo "N in kilocouter: $3"

#read dt

case "$2" in
    400 ) ss="00:00:02"
          mod="106"
          ;;
    800 ) ss="00:00:04"
          mod="210"
          ;;
    1600 ) ss="00:00:08"
           mod="435"
           ;;
    3200 ) ss="00:00:16"
           mod="840"
           ;;
    * ) printf '\a\t%s\n' 'Please input a valid deltat!' >&2
        exit 1 ;;

esac
ffmpeg -ss $ss -i $1 -vf "select=not(mod(n\, $mod))" -vsync vfr -q:v 2 $FOLDER/output%04d.jpg


python ~/Documents/KILOBOTS/kiloColors/RGBKiloCounter/kiloColorCounter.py -i $FOLDER -o "$FOLDER-results.csv" -r "$FOLDER-kilocounter" $3
