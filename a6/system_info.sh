#!/bin/sh
# author:Rongyi Chen
# date:3/9/2019

# get the number of cores 
coresCount=$([ $(uname) = 'Darwin' ] &&
		 sysctl -n hw.logicalcpu_max ||
		     lscpu -p | egrep -v '^#' | wc -l)
# get the free memory
freeMemory=$(free -m | awk 'NR==2{printf "%sMB(free)/%sMB(total) (%.2f%%)\n", $2-$3,$2,($2-$3)*100/$2 }')

# get the free space in storage
freeStorage=$(df -h | awk '$NF=="/"{printf "%dGB(free)/%dGB(total) (%.2f%%)\n", $2-$3,$2,(100-$5)}')

cat <<EOF
# cores: $coresCount
# memory: $freeMemory
# storage: $freeStorage 
EOF
