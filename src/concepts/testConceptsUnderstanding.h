#include <fstream>
#include <iostream>
#include "utility.h"

using UtilityNameSpace::myLog;
using UtilityNameSpace::splitLine;
using UtilityNameSpace::Splitter;

// TODO std::flush

namespace TestStopUsingSTDEndl {

/*
*
std::endl——Insert newline and flush
std::ends——Insert null character
std::flush——Flush stream buffer
*
1.What is Buffer?
Buffer is also known as cache, which is a part of memory space.That is to say,
a certain amount of storage space is reserved in the memory space which is used
to buffer input or output data, and this reserved space is called buffer.Buffers
are categorized into input buffers and output buffers depending on whether they
correspond to input or output devices.

2. Why do we need to introduce buffers?
For example, we take information from the disk, we first read out the data in
the buffer, the computer and then directly from the buffer to take the data,
such as buffer data to take the end of the disk and then go to read, so you can
reduce the number of times the disk read and write, coupled with the computer's
operation of the buffer is much faster than the operation of the disk, it is the
application of the buffer can greatly increase the speed of computer
operations.Another example, we use the printer to print documents, due to the
printer's printing speed is relatively slow, we first output the document to the
printer's corresponding buffer, the printer and then gradually print their own,
then our CPU can deal with other things. Now you basically understand it, the
buffer is a piece of memory, it is used between the input and output devices and
the CPU, used to cache data.It allows low-speed I/O devices and high-speed CPU
to work in harmony, avoiding low-speed I/O devices from occupying the CPU and
freeing the CPU to work efficiently.

3.Buffer Flush
Flushing a buffer is the process of taking the data in the buffer, performing
real I/O, and then emptying the buffer.As you can see from the example above,
the following situations trigger a buffer flush:
a. when the buffer is full;
b. at the end of the program;
c. by executing the flush statement;
d. by closing the file, i.e., by executing the close statement;
e. by executing the endl statement.

*/

void test() {
  std::ofstream outFile("test.txt");
  for (int n = 0; n < 4096; ++n) {
    outFile << 'A';
  }  // the data is still in buffer, not in the file. No I/O process.

  std::ofstream outFile1("test1.txt");
  for (int n = 0; n < 4096; ++n) {
    outFile1 << 'A';
  }
  outFile1 << std::endl;  // data in buffer has been writen into file.

  std::ofstream outFile2("test2.txt");
  for (int n = 0; n < 4096; ++n) {
    outFile2 << 'A';
  }
  outFile2.close();  // data in buffer has been writen into file.

  {
    std::ofstream outFile3("test3.txt");
    for (int n = 0; n < 4096; ++n) {
      outFile3 << 'A';
    }
    int a = 1;  // data in buffer has been writen into file.
  }

  int a = 1;
}

}  // namespace TestStopUsingSTDEndl

namespace TestNameDemangling {
/*
C++ Name Demangling is the process of restoring the "mangled" names of symbols
(e.g., function names, variable names, etc.) that are internally processed by
the compiler in C++ code to their original human-readable names.Since C++
supports function overloading and other advanced features, the compiler modifies
the names of symbols when generating the object code to uniquely identify each
symbol, a process called "name mangling".

*/

}  // namespace TestNameDemangling

namespace TestClangFormat {
/*
1. file name must be .clang-format. And put in the root directory of the project
2. enable clang format support in visual studio
3. Ctrl + K and Ctrl + D, apply code formating  for current file
*/

}

void test() {
  TestStopUsingSTDEndl::test();
}