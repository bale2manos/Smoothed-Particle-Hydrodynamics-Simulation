#!/bin/sh
. /etc/profile
module avail
module load gcc/12.1.0
valgrind --tool=cachegrind --cachegrind-out-file=salida_stats  ./build/fluid/fluid 5 small.fld out.fld
cg_annotate salida_stats ./fluid/fluid.cpp