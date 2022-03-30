#!/bin/bash

let n=40
let p1=10
let p2=10
let n1=2
let n2=3

#let seed=$RANDOM % 40

for t in 1600
#for t in 5000

do

  let q2=$n2*t

  for qbad in $n1
  do
    #let q1="$qbad*t" | bc
    let q1=$qbad*t

    let simtime=t*10
    #for l in 00 10 20 30 40 50 60 70 80
    for l in 20
    do
      echo "The value of \"t\" is $t."
      echo "The value of \"q1\" is $q1."
      for ((j=0; j<1; j++));
      do
        let k=j
        let seed=$RANDOM+1
        let ind_seed=$((1 + $RANDOM % 200))
        echo "the seed is $seed"
        echo "the ind_seed is $ind_seed"
        sed -i "s/\"nBots\" : .*,/\"nBots\" : $n,/g" kilombo.json
        sed -i "s/#define N AQUI/#define N $n/g" bees_nest_v02.h
        sed -i "s/#define NEST_ONE N1/#define NEST_ONE $n1/g" bees_nest_v02.h
        sed -i "s/#define NEST_TWO N2/#define NEST_TWO $n2/g" bees_nest_v02.h
        sed -i "s/#define PRIORI_ONE PI1/#define PRIORI_ONE $p1/g" bees_nest_v02.h
        sed -i "s/#define PRIORI_TWO PI2/#define PRIORI_TWO $p2/g" bees_nest_v02.h
        sed -i "s/#define QUALITY_ONE Q1/#define QUALITY_ONE $q1/g" bees_nest_v02.h
        sed -i "s/#define QUALITY_TWO Q2/#define QUALITY_TWO $q2/g" bees_nest_v02.h
        sed -i "s/#define DELTA_T T/#define DELTA_T $t/g" bees_nest_v02.h
        sed -i "s/#define LAMBDA INTERDEP/#define LAMBDA $l/g" bees_nest_v02.h
        sed -i "s/#define IND_SEED IS/#define IND_SEED $ind_seed/g" bees_nest_v02.h
        sed -i "s/\"randSeed\" : .*,/\"randSeed\" : $seed,/g" kilombo.json
        sed -i "s/\"stateFileSteps\" : .*,/\"stateFileSteps\" : $t,/g" kilombo.json
        sed -i "s/\"simulationTime\" : .*,/\"simulationTime\" : $simtime,/g" kilombo.json

        ndig=$(printf "%03d" $n)
        tdig=$(printf "%05d" $t)
        q1dig=$(printf "%02d" $n1)
        q2dig=$(printf "%02d" $n2)
        kdig=$(printf "%03d" $k)
        p2dig=$(printf "%02d" $p2)


        #sed -i "s/bees.json/bees_N=$n-p1=$p1-p2=$p2-q1=$q1-q2=$q2-lambda=$l-motion=inmovil_rep=$j.json/g" kilombo.json
        sed -i "s/bees.json/bees_model=2_v=1_N=$ndig-deltat=$tdig-lambda=$l-q1=$q1dig-q2=$q2dig-p1=$p1-p2=$p2dig-motion=movil-t=$simtime-seed=$seed-indseed=$ind_seed-rep=$kdig.json/g" kilombo.json
        #sed -i "/\stateFileName\" : .*,/\"stateFileName\" : 'bees_model=0_v=1_N=$n-deltat=$tdig-lambda=$l-q1=$q1dig-q2=$q2dig-p1=$p1-p2=$p2-motion=movil-t=$simtime-seed=$seed-rep=$j.json',/g":

        sed -i "s/\EXECUTABLE=.*/\  EXECUTABLE=bees_model=2_InitCycle=30_t=loop_deltat=$t-q1=$q1dig-q2=$q2dig-p1=$p1-p2=$p2-lambda=$l-color=RGB_beacons=2/g" Makefile
        #  make #
        make hex

        #make -f Makefile.osx


        sed -i "s/#define N $n/#define N AQUI/g" bees_nest_v02.h
        sed -i "s/#define PRIORI_ONE $p1/#define PRIORI_ONE PI1/g" bees_nest_v02.h
        sed -i "s/#define PRIORI_TWO $p2/#define PRIORI_TWO PI2/g" bees_nest_v02.h
        sed -i "s/#define NEST_ONE $n1/#define NEST_ONE N1/g" bees_nest_v02.h
        sed -i "s/#define NEST_TWO $n2/#define NEST_TWO N2/g" bees_nest_v02.h
        sed -i "s/#define QUALITY_ONE $q1/#define QUALITY_ONE Q1/g" bees_nest_v02.h
        sed -i "s/#define QUALITY_TWO $q2/#define QUALITY_TWO Q2/g" bees_nest_v02.h
        sed -i "s/#define LAMBDA $l/#define LAMBDA INTERDEP/g" bees_nest_v02.h
        sed -i "s/#define DELTA_T $t/#define DELTA_T T/g" bees_nest_v02.h
        sed -i "s/#define IND_SEED $ind_seed/#define IND_SEED IS/g" bees_nest_v02.h
        sed -i "s/bees_model=2_v=1_N=$ndig-deltat=$tdig-lambda=$l-q1=$q1dig-q2=$q2dig-p1=$p1-p2=$p2dig-motion=movil-t=$simtime-seed=$seed-indseed=$ind_seed-rep=$kdig.json/bees.json/g" kilombo.json

        #rm *.o
        #rm ./bees_model=2_v=1_loop_deltat=1000
      done
    done
    #cp *deltat=$tdig* DataToReadInR/
    #Rscript ../BotsSeenData_sep2021.R
    #Rscript ../convertJSONtoCSV.R
    #rm DataToReadInR/*.json
    #rm bees_model=2_v=1_loop_deltat=1000
  done
done
