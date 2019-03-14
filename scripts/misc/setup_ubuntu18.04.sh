#!/bin/sh
# @file scripts/misc/setup_ubuntu18.04.sh
#
# @author Hiroyuki Chishiro
#

prefix="sudo apt-get -y install"

# compiler
$prefix ccache gcc clang clang-tools llvm gcc-aarch64-linux-gnu

# C analyze tool
$prefix cppcheck flawfinder

# python
$prefix python3 python3-pip
# python analyze tool
pip3 install pyflakes pylint 

# tool
$prefix git samba make doxygen sendmail git genisoimage grub-efi kpartx

# LaTeX
$prefix tetex-* texlive-latex* texlive-science ptex-jtex
$prefix texlive-lang-japanese texlive-fonts-recommended
$prefix dvi2ps-fontdesc-morisawa5   

# qemu
$prefix qemu qemu-system


echo "\n"
echo "If you use Qemu 2.12.0 (or later) for Raspberry Pi3 (ARM64), please see doc/user.tex"
echo "If you use AXIS ISA for DOM, please use https://github.com/pflab-ut/llvm"
