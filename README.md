# memtool

This tiny header-only C++17 project to enhance working with memory as of `std::vector<std::byte>`: 

* for a hex to memory and vice versa conversion,
* dump a canonical representation of the memory,
* manipulate the memory as a stream

This project is heavily inspired by my own libait-memory library I created for the 
AIT, Austrian Institute of Technology


## Usage example

A memtool comes along with these methods and functions:

* `headcode::memtool::CharArrayToMemory` --> convert an old-school C character array to `std::vector<std::byte>`
* `headcode::memtool::HexToMemory` --> convert a hex string to `std::vector<std::byte>` like
    ```c++
    std::vector<std::byte> data = headcode::memtool::HexToMemory("dead1337");
    ```
* `headcode::memtool::MemoryToCanonicalString` --> dumps a canonical representation of the memory, like
    ```c++
    std::vector<std::byte> data;
    std::cout << headcode::memtool::MemoryToCanonicalString(data) << std::endl;
    ```
    will give something similar to
    ```
    0x0000000000000010   10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f   |........ ........|
    0x0000000000000020   20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d 2e 2f   | !"#$%&' ()*+,-./|
    ...
    ```
* `headcode::memtool::MemoryToHex` --> converts a memory to a hex string.
* `headcode::memtool::StringToMemory` --> convenient method for quick conversion of a std::string.

Additional memtool provides a `MemoryManipulator` class which can be used to
stream in and out data into a memory. Hence, this `MemoryManipulator` does not take
ownership of the memory.

Example:
```c++
std::vector<std::byte> data;
headcode::memtool::MemoryManipulator buffer{data};

buffer << std::string{"foo"};
buffer << 3.1415 << 42 << 'b' << 'a' << 'r';

std::vector<int> blah{45, -12, 10, 0};
buffer << blah;

std::cout << data.size() << std::endl;      // will hold the bytes needed fot the above data streamed
```

Stream in and out is provided.


## Project layout

```
.
├── 3rd                         3rd party libraries needed (likely as git submodules).
├── cmake                       CMake additional files.
│   ├── deb-package.cmake       Building instructions for DEB packages.
│   └── rpm-package.cmake       Building instructions for RPM packages.
├── include                     Public header files. Add the path to this folder to your C++ search path.
│   └── headcode                
│       └── memtool             Include this: <headcode/memtool/memtool.hpp>
├── test                        Tests.
│   ├── benchmark               Benchmark tests.
│   ├── integration             Integration tests.
│   └── unit                    Unit tests.
├── tools                       Various tools for run-time or build-time.
│   └── docker                  Docker builder image definitions: Dockerfiles for various platforms to build.
├── Changes.md                  Changes file.
├── CMakeLists.txt              The overall CMakeLists.txt.
├── Doxyfile                    Doxgen API documentation configuration.
├── LICENSE.txt                 The software license.
└── README.md                   This file.
```

## Build

### Dependencies

- cmake
- gcc (with g++) or clang (witch clang++)
- git
- make
- doxygen (with graphviz)
- [googletest](https://github.com/google/googletest) (as submodule)

When cloning this project execute the following to clone submodules as well:

```bash
$ git submodule init
$ git submodule update
```

or simply clone with the `--recurse-submodule` option:
```bash
$ git clone --recurse-submodules
```

#### Native build

memtool is a [cmake](https://cmake.org) project with out-of-source builds in
a dedicated folder, usually labeled "build".

```bash
$ mkdir build && cd build
$ cmake ..
$ make
```

## Test

After compilation run ctest
```bash
$ cd build
$ ctest
```
Or
```bash
$ cd build
$ make test
```

_Note: Please check the test files for documentation. 
The tests are easy to read and tell you how the code is intended to be used._ 

### Test Coverage

You may also run in-deep test coverage profiling. For this, you have to turn on profiling mode:
```bash
$ cd build
$ cmake -D PROFILING_MODE_ENABLED=on ..
```

Then compile as usual and run the tests. After the tests make the `run-gcovr` target: 
```bash
$ make test
$ make run-gcovr
```

This will give you the test coverage on stdout as well as:
* `gcovr-coverage.info`:  this is the coverage info file created by gcovr
* `gcovr-report.xml`: this is the gcovr report file in xml
* `coverge-html`: this is the folder in which detailed html info of collected coverage resides
  (open up the file `coverage-html/index.html` in a browser of your choice)

in the build folder.


## Notable guidelines

* Coding Guidelines: https://google.github.io/styleguide/cppguide.html
* How (not) to write git commit messages: https://www.codelord.net/2015/03/16/bad-commit-messages-hall-of-shame/
* How to version your software: https://semver.org/
* How to write a clever "Changes" file: https://keepachangelog.com/en/1.0.0/
* Folder Convention: https://github.com/KriaSoft/Folder-Structure-Conventions

---

(C)opyright 2020 headcode.space
[https://headcode.space](https://www.headcode.space)

