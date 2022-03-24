#!/bin/bash

echo "filename: $1"
echo "dir: $2"
echo "deltat: $3"
#read dt

case "$3" in
    400 ) ss="00:00:02"
          mod="106"
          ;;
    800 ) ss="00:00:04"
          mod="210"
          ;;
    1600 ) ss="00:00:08"
           mod="420"
           ;;
    3200 ) ss="00:00:16"
           mod="840"
           ;;
    * ) printf '\a\t%s\n' 'Please input a valid deltat!' >&2
        exit 1 ;;

esac

ffmpeg -ss $ss -i $1 -vf "select=not(mod(n\, $mod))" -vsync vfr -q:v 2 $2/output%04d.jpg
