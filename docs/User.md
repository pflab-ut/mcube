# User

## Makefile


|Makefile Commands|Descriptions|
|:--|--:|
|`make all`|build target file|
|`make buildclean`|clean build files and directories|
|`make clean`|clean build and doxygen files and directories|
|`make cmrun`|execute Mcube Kernel with `MONITOR_ALL` and `PRINT_ALL` in AXIS cluster|
|`make configure`|do user-defined configuration|
|`make cppcheck`|analyze codes by `cppcheck`|
|`make crun`|execute Mcube Kernel with `PRINT_ALL` in AXIS cluster|
|`make defaultconfig`|do default configuration|
|`make docker`|build target file with docker|
|`make doxygen`|create doxygen file|
|`make doxygenclean`|clean doxygen files and directories|
|`make flawfinder`|analyze codes by `flawfinder`|
|`make grun`|execute Mcube Kernel with GUI|
|`make line`|output the number of lines in C, header, and python files|
|`make mrun`|execute Mcube Kernel with `MONITOR_ALL` and `PRINT_ALL` in AXIS|
|`make pyflakes`|analyze codes by `pyflakes`|
|`make pylint`|analyze codes by `pylint`|
|`make run`|execute Mcube Kernel with CUI|
|`make scan-build`|analyze codes by `scan-build`|
|`make setup`|setup build environments|
|`make size`|list secton sizes and total sizes of Mcube Kernel|
|`make testconfig`|do build tests for all configurations|


## How to Test Configuration

First of all please set following environment variables for your e-mail address.

```
$ export EMAIL_FROM_ADDRESS=YOUR_EMAIL_FROM_ADDRESS
$ export EMAIL_TO_ADDRESS=YOUR_EMAIL_TO_ADDRESS
```

Then please execute the following command.

```
$ make testconfig
```
