#!/bin/sh
# @file scripts/misc/aixs_cluster_run.sh
#
# @author Hiroyuki Chishiro
#

#irun="bsub -Ip -q incisive irun"
#irun="irun"
#irun="irun -64bit"
irun="irun -64bit +define+USE_CPU0_SINGLE +define+USE_CPU1_SINGLE "$1

#irun = "${irun}$1"
echo $irun

if true
#if false   
then
    
    topdir="../sim.lap_top/rtl"

    $irun \
        +sv \
        +incdir+$topdir/include \
        $topdir/lap_top_test.sv \
        $topdir/*.svp
    
else
    
    topdir="../sim.rx0.ut180817/rtl"
    $irun \
        +sv \
        +access+r \
        +ncnowarn+NONPRT \
        +ncnowarn+WARIPR \
        +define+PRINT_ALL \
        +incdir+$topdir/include \
        $topdir/*.sv \
        $topdir/*.svp
fi
   
