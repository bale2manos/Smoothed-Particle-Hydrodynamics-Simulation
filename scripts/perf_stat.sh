#!/bin/sh
. /etc/profile
module avail
module load gcc/12.1.0
perf stat -r 5 ./build-release/fluid/fluid 1000 ./in/large.fld out.fld