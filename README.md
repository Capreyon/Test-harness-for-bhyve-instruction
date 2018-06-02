
## Introduction

Bhyve (beehive) is a type2 hypervisor that runs on FreeBSD. bhyve runs FreeBSD 9+, OpenBSD, NetBSD, Linux and Windows NT--guests. Current development efforts aim at widening support for other operating systems for the x86-64 architecture.
It runs guest operating system inside a virtual machine (parameters like the number of CPUs, amount of guest memory and input-output connectivity can be specified with command line parameter).
It has a software emulator which needs to emulate some Instructions. Bhyve Test harness is a project of Gsoc 2k18. As it's name suggests, it's purpose is to provide tests for Instructions emulated by bhyve. The tests are tiny guest operating system 
that generally executes tens of lines of C and assembler test code in order to obtain its Pass/Fail result. With the help of Test harness we can compare the instructions emulated by bhyve and XED. Tests provide bhyve and virtual hardware functional testing by targeting the features through minimal implementations of their use per the hardware specification. The simplicity of tests make them easy to verify they are correct, easy to maintain, and easy to use in timing measurements. Tests are also often used for quick and dirty bug reproducers. The reproducers may then be kept as regression tests.
While a single Test is focused on a single feature, all Tests share the minimal system initialization and setup code. There are also several functions, stubs, drivers made shareable across all Tests, comprising a Test API.

### Prerequisites 

Bhyve Instructions emulation can not only run with bhyve but also in userspace. So you can compare instructions by bhyve and Using XED. For using XED see here [Intel_XED](https://intelxed.github.io)  

