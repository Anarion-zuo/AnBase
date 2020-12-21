# AnBase: My C++ Library
This is my side project for common utilities in C++. Although there is STL, boost, and all that, I am perfecting this project for some basic practise in programming, and for a bit of thrill.

This project is developped under Linux. It might compile on some other Unix platform, with some tiny alteration. It would never compile under Windows without heavy refactoring. Nevertheless, I would recommand you to abandon Windows entirely as your developping environment, regardless of WSL.

# Compile
If you happen to be interested in my humble project, here is how you would compile it.

I would recommand that you use Linux, if you have not already done so. Other Unix platform may suffice, although you might have to adjust a little. If the problem of platform does not stop you from compiling my project, please do commit your code to this project. You may implement platform-dependent compile option with Macros.

I confess that I have little `Makefile` experience. If you may kindly make my `Makefile` better, I would be most grateful.

The project is mainly built with CMake, for CLion has excellent support. To compile directly, you may clone the repository and `cd` into it.
 
Run `make dist` for a release version. There would be a new directory `dist` created under the repositories' root directory, containing a static library file `libanbase.a`. You must give flags to the compiler when linking or including `anbase`.
 
Run `make sysdist` for `installation`. The command compiles and copies `libanbase.a` to `/usr/local/lib`, and directory `include` to `/usr/local/include/anbase`. After `sysdist`, you may conveniently link `libanarion.a` and include `anbase` from anywhere in the system without adding flags to the compiler.