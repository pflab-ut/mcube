#!/bin/sh
# @file scripts/misc/run_axis_cluster.sh
#
# @author Hiroyuki Chishiro
#

#irun="bsub -Ip -q incisive irun"
#irun="irun"
#irun="irun -64bit"
irun="irun -64bit +define+USE_CPU0_SINGLE +define+USE_CPU1_SINGLE "$1

irun = "${irun}$1"
#echo $irun

topdir="../sim.rx0.ut180817/rtl"
$irun \
    +sv \
    +access+r \
    +ncnowarn+NONPRT \
    +ncnowarn+WARIPR \
    +incdir+$topdir/include \
    $topdir/*.sv \
    $topdir/*.svp

