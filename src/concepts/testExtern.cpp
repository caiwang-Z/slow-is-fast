#include "testExtern.h"

extern int g_configValue = 199;
// Other source files referenced simply include the header file:
// This way the compiler knows that g_configValue is a global variable and the linker corresponds the reference to the
// definition.

/*
But extern "C" is used when you need to specify a C link:
// Declaring C Function Interfaces in C++ Code
extern "C" {
    void c_function(int);
}
This way the compiled symbols will not be name-mangled by C++, making it easier to link with C code or libraries in
other languages.
*/