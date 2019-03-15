#!/bin/sh
# author:Rongyi Chen
# date:3/9/2019
# I used the following resources to implement this code:
# https://stackoverflow.com/a/23378780
# https://unix.stackexchange.com/a/119170

# get the number of cores 
coresCount=$(nproc)
# get the free memory
freeMemory=$(free -m | awk 'NR==2{printf "%s MB(free)/%s MB(total)\n", $2-$3,$2 }')

# get the free space in storage
freeStorage=$(df -h | awk '$NF=="/"{printf "%d GB(free)/%d GB(total)\n", $2-$3,$2 }')

cat <<_EOF_
  cores: $coresCount
  memory: $freeMemory
  storage: $freeStorage 
_EOF_
