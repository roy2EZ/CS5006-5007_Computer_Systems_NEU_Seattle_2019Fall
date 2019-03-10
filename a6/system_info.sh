#!/bin/sh

# Number of cores (includes those reported by hyper-threading cores)
# Linux: Simply count the number of (non-comment) output lines from `lscpu -p`, 
# which tells us the number of *logical* CPUs.
coresCount=$([ $(uname) = 'Darwin' ] &&
		 sysctl -n hw.logicalcpu_max ||
		     lscpu -p | egrep -v '^#' | wc -l)

freeMemory=$(free -m | awk 'NR==2{printf "%sMB(free)/%sMB(total) (%.2f%%)\n", $2-$3,$2,($2-$3)*100/$2 }')

freeStorage=$(df -h | awk '$NF=="/"{printf "Disk Usage: %dGB/%dGB (%.2f%%)\n", $2-$3,$2,(100-$5)}')
cat <<EOF
# cores: $coresCount
# memory: $freeMemory
# storage: $freeStorage 
EOF
