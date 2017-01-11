#!/bin/bash

TYPE=$1
PARAM=$2
mkdir -p "new-$TYPE"
for f in new-$TYPE/$PARAM-*.in; do
	regex="new-.*-(.*).in"
	if [[ $f =~ $regex ]]; then
		n=${BASH_REMATCH[1]}
		echo -n "$n "
		/usr/bin/time -f "%e" ./weisfeiler-lehman w $f > /dev/null
	fi
done &>> results/time-$TYPE-$PARAM

