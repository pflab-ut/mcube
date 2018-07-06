# Mcube Kernel
Operating System for Micro/Monotlithic/MultiKernels

## Description
Mcube Kernel is an operating system for Micro/Monotlithic/MultiKernels.


## Supports for Cross Development (Ubuntu 18.04 LTS)
- LLVM-6.0.0/GCC-7.3.0 for X86 and SIM (user-level)
- LLVM-6.0.0/GCC-4.8.4 Extension for AXIS ISA
      - LLVM's ISA name is MAXIS
      - GCC's ISA name is DOM
- Python 3.6.5 (Python 3.6 or higher)


## Set up Configuration
Please edit "configure" file.
Explanations of "configure" file is in "Kconfig".


## How to Build

If you modify "configure" file, please execute the following command.
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
