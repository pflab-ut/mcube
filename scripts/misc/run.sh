#!/bin/sh
# @file scripts/misc/run.sh
#
# @author Hiroyuki Chishiro
#

#irun="bsub -Ip -q incisive irun"
#irun="irun"
#irun="irun -64bit"
irun="irun -64bit +define+USE_CPU0_SINGLE +define+USE_CPU1_SINGLE "$1

#irun = "${irun}$1"
echo $irun

topdir="../sim.lap_top/rtl"

$irun \
    +sv \
    +incdir+$topdir/include \
    $topdir/lap_top_test.sv \
    $topdir/*.svp
