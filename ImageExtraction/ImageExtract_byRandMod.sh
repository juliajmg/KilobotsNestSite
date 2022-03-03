#!/bin/bash

declare -a SelectedFrames=("eq(n\\, 0)+")

video=ColorSyncTest_deltat=400iterations_IS0=100_diafragma=08_RGB_beacons=0_sample=03.mp4
ireset=0
#total=26474

total=26578
maxf=0
mod=95


#for i in {0..26474}
while [ $(($maxf + $mod)) -le $total ];
do
  rand=$(($RANDOM % 4))

  if [ $rand -eq 0 ]
  then
    mod=97
  elif [ $rand -eq 1 ]
  then
    mod=98
  elif [ $rand -eq 2 ]
  then
    mod=99
  else
    mod=100
  fi

  maxf=$(($maxf + $mod))

  iprep=$(printf "eq(n\\, $maxf)+")
  SelectedFrames=("${SelectedFrames[@]}" $iprep)
done

echo ${#SelectedFrames[@]}
bar=$(printf "%s" "${SelectedFrames[@]}")
#bar=${bar:1}
bar=${bar::-1}

echo $bar



ffmpeg -ss 00:00:01.92 -i $video -vf "select='$bar'" -vsync 0 -q:v 2 images/RGB/beacons=0/sample=03/out%3d.jpg
