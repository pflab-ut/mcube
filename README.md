# Mcube Kernel
Operating System Supporting Dynamically Switchable Kernel Models

## Description
Mcube Kernel is an operating system for Micro/Monotlithic/MultiKernels.
Dynamically switchable kernel models can avoid unnecessary recompile and
reboot.

![Mcube Kernel](docs/fig/mcube.jpg "Mcube Kernel")

## Supporting Compilers and ISAs
- LLVM-6.0.0/GCC-7.3.0 (or later) for X86_64, ARM64 (AARCH64), and SIM (user-level) ISAs
- LLVM-6.0.0/GCC-4.8.4 Extension for AXIS ISA
  - LLVM's ISA name is MAXIS (can be downloaded from https://github.com/pflab-ut/llvm)
  - GCC's ISA name is DOM (not open for public)
- Python 3.6.5 (or later)


## Set up Build Environment (Ubuntu 18.04 LTS)

Please execute the following command (root privilege required).
```
$ make setup
```

## Set up Configuration

Please edit `configure` file or execute the following command.
```
$ cp docs/config/ARCH_configure configure
```
where `ARCH_configure` is `aarch64_raspi3_configure`, `aarch64_synquacer_configure`,
`axis_configure`, `sim_configure`, or `x86_64_configure`.

`ARCH` of `ARCH_configure` is changed to an architecture name of small
characters.
Details of `configure` file are explained in `Kconfig`.

## How to Build

First of all you check and modify `configure` file.
If `CONFIG_XXX=y`, enable the configuration.
If `CONFIG_XXX=n`, disable the configuration.
Then please execute the following command.
```
$ make configure
```

Please execute the following command.
```
$ make
```

Otherwise if you would like to use `docker`, please execute the following command.
```
$ make docker
```


If cleaning build files, please execute the following command.
```
$ make clean
```

## How to Execute

### Without Arguments

Please execute the following command in CUI.
```
$ make run
```

Please execute the following command in GUI.
```
$ make grun
```

### With Arguments in SIM Architecture

Please execute the following command in CUI.
```
$ make run ARGS="arg1 arg2 arg3"
```

Please execute the following command in GUI.
```
$ make grun ARGS="arg1 arg2 arg3"
```

## Documentations
* [System](docs/System.md)
* [User](docs/User.md)
* [Misc](docs/Misc.md)
* [Acknowledgments](docs/Acknowledgments.md)


## License
- 2-clause BSD License
- See LICENSE

## Copyright
2018-2019 Hiroyuki Chishiro
