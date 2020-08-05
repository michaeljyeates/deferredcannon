#!/bin/bash

CLEOS=/home/mike/Projects/EOS/jungle.sh

TIME=$1
COUNT=500
ITR=100
DATA_SIZE=500000

i=0
while [ $i -lt $ITR ]
do
RND=$((1 + RANDOM % 1000000000000))
RND2=$((1 + RANDOM % 1000000000000))
echo "[$COUNT, $RND$RND2, $TIME, $DATA_SIZE]" > load.json
$CLEOS push action ohnoesnothim l load.json -p ohnoesnothim
i=$[$i+1]
sleep 0.1
done
