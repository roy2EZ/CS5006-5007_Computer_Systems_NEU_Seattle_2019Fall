#!/bin/sh

# Number of cores (includes those reported by hyper-threading cores)
# Linux: Simply count the number of (non-comment) output lines from `lscpu -p`, 
# which tells us the number of *logical* CPUs.
coresCount=$([ $(uname) = 'Darwin' ] && 
                       sysctl -n hw.logicalcpu_max || 
                       lscpu -p | egrep -v '^#' | wc -l)

cat <<EOF
# cores: $coresCount
EOF
