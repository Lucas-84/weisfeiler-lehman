#!/bin/bash

TYPE=$1
OFFSET=$2
MAXSIZE=$3
PARAM=$4
mkdir -p "new-$TYPE"
for i in `seq $OFFSET $OFFSET $MAXSIZE`; do
	EXEC="gen-$TYPE"
	#X=`bc <<< "scale=20; $PARAM/$i"`
	X=$(( $i - 1 ))
	./$EXEC $i $X > "new-$TYPE/$PARAM-$i.in"
done

