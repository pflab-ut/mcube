# Mcube Kernel
Operating System Supporting Dynamically Switchable Kernel Models

## Description
Mcube Kernel is an operating system for Micro/Monotlithic/MultiKernels.
Dynamically switchable kernel models avoid unnecessary recompile and
reboot.


## Supporting ISAs for Cross Development (Ubuntu 18.04 LTS)
- LLVM-6.0.0/GCC-7.3.0 for X86, ARM, and SIM (user-level) ISAs (or later)
- LLVM-6.0.0/GCC-4.8.4 Extension for AXIS ISA
  - LLVM's ISA name is MAXIS (download from https://github.com/pflab-ut/llvm)
  - GCC's ISA name is DOM (not open for public)
- Python 3.6.5 (or higher)


## Set up Configuration
Please edit "configure" file.
Explanations of "configure" file is in "Kconfig".


## How to Build

First of all you check and modify "configure" file, and then please
execute the following command.
```
$ make configure
```

Please execute the following command.
```
$ make
```
Then, create "mcube" file in the top directory of "mcube".


If cleaning build files, please execute the following command.
```
$ make clean
```

## How to Execute

Please execute the following command.
```
$ make run
```


## How to Test Configuration

Please set following environment variables for your e-mail address.

```
export EMAIL_FROM_ADDRESS=YOUR_EMAIL_FROM_ADDRESS
export EMAIL_TO_ADDRESS=YOUR_EMAIL_TO_ADDRESS
```

Please execute the following command.

```
$ make testconfig
```

After test configuration, send results of test configuration to your e-mail.


## License
- 2-clause BSD License
- See LICENSE

## Copyright
2018 Hiroyuki Chishiro
