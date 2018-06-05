
## Introduction

Bhyve (beehive) is a type2 hypervisor that runs on FreeBSD. bhyve runs FreeBSD 9+, OpenBSD, NetBSD, Linux and Windows NT--guests. Current development efforts aim at widening support for other operating systems for the x86-64 architecture.
It runs guest operating system inside a virtual machine (parameters like the number of CPUs, amount of guest memory and input-output connectivity can be specified with command line parameter).
It has a software emulator which needs to emulate some Instructions. Bhyve Test harness is a project of Gsoc 2k18. As it's name suggests, it's purpose is to provide tests for Instructions emulated by bhyve. The tests are tiny guest operating system 
that generally executes tens of lines of C and assembler test code in order to obtain its Pass/Fail result. With the help of Test harness we can compare the instructions emulated by bhyve and XED. Tests provide bhyve and virtual hardware functional testing by targeting the features through minimal implementations of their use per the hardware specification. The simplicity of tests make them easy to verify they are correct, easy to maintain, and easy to use in timing measurements. Tests are also often used for quick and dirty bug reproducers. The reproducers may then be kept as regression tests.
While a single Test is focused on a single feature, all Tests share the minimal system initialization and setup code. There are also several functions, stubs, drivers made shareable across all Tests, comprising a Test API.

### Prerequisites 

Bhyve Instructions emulation can not only run with bhyve but also in userspace. So you can compare instructions by bhyve and Using XED. For using XED see here [Intel_XED](https://intelxed.github.io) 

### Run tests

For comparing the bhyve emulation with XED run the bhyve emulation in Userland/Userspace and for running the instructions and tests using XED clone the [XED repository](https://github.com/intelxed/xed) from here.

## Abbreviated building instructions:


git clone https://github.com/intelxed/xed.git xed
git clone https://github.com/intelxed/mbuild.git mbuild
cd xed
./mfile.py
then get your libxed.a from the obj directory. Add " --shared" if you want a shared object build. Add " install" if you want the headers & libraries put in to a kit in the "kits" directory. Add "C:/python27/python " before "./mfile.py" if on windows.

### How to build the examples:

There are two options:

1.When building libxed you can also build the examples, from the main directory (above examples):

./mfile.py examples

and the compiled examples will be in obj/examples.

2. Build a compiled "kit" and the build the examples from within the kit:

./mfile.py install cd kits cd cd examples ./mfile.py

See source build documentation for more information.

3. Then go to xed/obj/examples and run the xed file using ./xed --help. You will get a list of encoding/decoding options.
   Select -ir raw_input_file (decode a raw unformatted binary file) and run the tests.
   
   
   
                                 Tests are in developing process



