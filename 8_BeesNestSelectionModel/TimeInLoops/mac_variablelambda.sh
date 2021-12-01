#!/bin/bash

let n=100
let p1=25

let p2=25

#let seed=$RANDOM % 40

for t in 100 


do
let q1=16*t
let q2=30*t
let simtime=20*t
#for l in 00 05 10 15 20 25 30 35 40 45 50 55 60 65 70 75 80
for l in 00
do
  echo "The value of \"t\" is $t."
  echo "The value of \"q1\" is $q1."
  for ((j=0; j<2; j++));
  do
    let seed=$RANDOM+1
    let k=j
    echo "the seed is $seed"
    sed -i '' "s/\"nBots\" : .*,/\"nBots\" : $n,/g" kilombo.json
    sed -i '' "s/#define N AQUI/#define N $n/g" bees_nest_v01.h
    sed -i '' "s/#define PRIORI_ONE PI1/#define PRIORI_ONE $p1/g" bees_nest_v01.h
    sed -i '' "s/#define PRIORI_TWO PI2/#define PRIORI_TWO $p2/g" bees_nest_v01.h
    sed -i '' "s/#define QUALITY_ONE Q1/#define QUALITY_ONE $q1/g" bees_nest_v01.h
    sed -i '' "s/#define QUALITY_TWO Q2/#define QUALITY_TWO $q2/g" bees_nest_v01.h
    sed -i '' "s/#define TIME_UNIT T/#define TIME_UNIT $t/g" bees_nest_v01.h
    sed -i '' "s/#define LAMBDA INTERDEP/#define LAMBDA $l/g" bees_nest_v01.h
    sed -i '' "s/\"randSeed\" : .*,/\"randSeed\" : $seed,/g" kilombo.json
    sed -i '' "s/\"stateFileSteps\" : .*,/\"stateFileSteps\" : $t,/g" kilombo.json
    sed -i '' "s/\"simulationTime\" : .*,/\"simulationTime\" : $simtime,/g" kilombo.json


    tdig=$(printf "%05d" $t)
    q1dig=$(printf "%06d" $q1)
    q2dig=$(printf "%06d" $q2)
    kdig=$(printf "%03d" $k)

    #sed -i "s/bees.json/bees_N=$n-p1=$p1-p2=$p2-q1=$q1-q2=$q2-lambda=$l-motion=inmovil_rep=$j.json/g" kilombo.json
    sed -i '' "s/bees.json/bees_model=1_v=1_N=$n-deltat=$tdig-lambda=$l-q1=$q1dig-q2=$q2dig-p1=$p1-p2=$p2-motion=movil-t=$simtime-seed=$seed-rep=$kdig.json/g" kilombo.json
    #sed -i "/\stateFileName\" : .*,/\"stateFileName\" : 'bees_model=0_v=1_N=$n-deltat=$tdig-lambda=$l-q1=$q1dig-q2=$q2dig-p1=$p1-p2=$p2-motion=movil-t=$simtime-seed=$seed-rep=$j.json',/g":

    #make #
    make -f Makefile.osx
    ./bees_nest_v01

    sed -i '' "s/#define N $n/#define N AQUI/g" bees_nest_v01.h
    sed -i '' "s/#define PRIORI_ONE $p1/#define PRIORI_ONE PI1/g" bees_nest_v01.h
    sed -i '' "s/#define PRIORI_TWO $p2/#define PRIORI_TWO PI2/g" bees_nest_v01.h
    sed -i '' "s/#define QUALITY_ONE $q1/#define QUALITY_ONE Q1/g" bees_nest_v01.h
    sed -i '' "s/#define QUALITY_TWO $q2/#define QUALITY_TWO Q2/g" bees_nest_v01.h
    sed -i '' "s/#define LAMBDA $l/#define LAMBDA INTERDEP/g" bees_nest_v01.h
    sed -i '' "s/#define TIME_UNIT $t/#define TIME_UNIT T/g" bees_nest_v01.h
    sed -i '' "s/bees_model=1_v=1_N=$n-deltat=$tdig-lambda=$l-q1=$q1dig-q2=$q2dig-p1=$p1-p2=$p2-motion=movil-t=$simtime-seed=$seed-rep=$kdig.json/bees.json/g" kilombo.json

    rm *o
    rm ./bees_nest_v01

  done
done

cp *deltat=$tdig* ../DataToReadinR/

Rscript convertJSONtoCSV.R
rm ../DataToReadinR/*.json
rm bees_model=1_v=1_N=100-deltat=*
done
