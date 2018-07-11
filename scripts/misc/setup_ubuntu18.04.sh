#!/bin/sh
# -*- coding: utf-8 -*-
# @file scripts/misc/setup_ubuntu18.04.sh
#
# @author Hiroyuki Chishiro
#

prefix="sudo apt-get -y install"

# compiler
$prefix ccache gcc clang gcc-aarch64-linux-gnu nasm

# sendmail
$prefix sendmail

# python
$prefix python3 python3-pip
pip3 install pyflakes pylint 

# git
$prefix git

# samba
$prefix samba

# Doxygen
$prefix doxygen


# LaTeX
$prefix tetex-* texlive-latex* texlive-science ptex-jtex
$prefix okumura-clsfiles xdvik-ja texlive-fonts-recommended
$prefix vfdata-morisawa5 dvi2ps-fontdesc-morisawa5

# qemu
$prefix qemu qemu-system


echo "\n"
echo "If you use Qemu 2.12.0 for Raspberry Pi3 (ARM64), please see doc/user.tex"
echo "If you use AXIS ISA for DOM, please use https://github.com/pflab-ut/llvm"
