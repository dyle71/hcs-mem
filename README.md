# hcs-mem

This tiny header-only C++17 project to enhance working with memory as of `std::vector<std::byte>`: 

* for a hex to memory and vice versa conversion,
* dump a canonical representation of the memory,
* manipulate the memory as a stream

This project is heavily inspired by my own libait-memory library I created for the 
AIT, Austrian Institute of Technology, some time ago in the past.

Rational: `std::byte` is a good idea, but - heck! - sometimes cumbersome to work with. And sometimes you just 
want to dump a memory area to `stderr` to inspect what is going on (or not going on for that matter).


## Usage example

hcs-mem comes along with these methods and functions:

* `headcode::mem::CharArrayToMemory` --> convert an old-school C character array to `std::vector<std::byte>`
* `headcode::mem::HexToMemory` --> convert a hex string to `std::vector<std::byte>` like
    ```c++
    std::vector<std::byte> data = headcode::mem::HexToMemory("dead1337");
    ```
* `headcode::mem::MemoryToCanonicalString` --> dumps a canonical representation of the memory, like
    ```c++
    std::vector<std::byte> data;
    std::cout << headcode::mem::MemoryToCanonicalString(data) << std::endl;
    ```
    will give something similar to
    ```
    0x0000000000000010   10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f   |........ ........|
    0x0000000000000020   20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d 2e 2f   | !"#$%&' ()*+,-./|
    ...
    ```
* `headcode::mem::MemoryToHex` --> converts a memory to a hex string.
* `headcode::mem::StringToMemory` --> convenient method for quick conversion of a std::string.
* Of course old-school C arrays are supported too:
    ```c++
    char * p = new char[1024];
    ...
    std::cout << headcode::mem::CharArrayToCanonicalString(p, 1024, "foo: ") << std::endl;
    ```
    will give something similar to
    ```
    foo: 0x0000000000000010   10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f   |........ ........|
    foo: 0x0000000000000020   20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d 2e 2f   | !"#$%&' ()*+,-./|
    ...
    ```


Additional mem provides a `MemoryManipulator` class which can be used to
stream in and out data into a memory. Hence, this `MemoryManipulator` does not take
ownership of the memory.

Example:
```c++
std::vector<std::byte> data;
headcode::mem::MemoryManipulator buffer{data};

buffer << std::string{"foo"};
buffer << 3.1415 << 42 << 'b' << 'a' << 'r';

std::vector<int> blah{45, -12, 10, 0};
buffer << blah;

std::cout << data.size() << std::endl;      // will hold the bytes needed fot the above data streamed
```

Stream in and out is provided.

**NOTE** I'm using the Googletest Suite for my test. Therefore I strongly recommend 
examining the source code in the test, since they are also meant for documentation 
and show how this code is intended to be used. Look for instance at this:

```c++
#include <gtest/gtest.h>
#include <headcode/mem/mem.hpp>
using namespace headcode::mem;

TEST(Memory, CharArrayToMemory) {

    auto text = "The quick brown fox jumps over the lazy dog";
    auto memory = CharArrayToMemory(text, strlen(text));

    auto canonical = MemoryToCanonicalString(memory);
    auto expected =
            "\
0x0000000000000000   54 68 65 20 71 75 69 63  6b 20 62 72 6f 77 6e 20   |The quic k brown |\n\
0x0000000000000010   66 6f 78 20 6a 75 6d 70  73 20 6f 76 65 72 20 74   |fox jump s over t|\n\
0x0000000000000020   68 65 20 6c 61 7a 79 20  64 6f 67                  |he lazy  dog     |\n";

    EXPECT_STREQ(canonical.c_str(), expected);
}
```
IMHO this is very clear and expressive. 


## Philosophy

All `headcode.space` software follows these directives in that order:

1. Provide very **high quality** C++ software: the software does what it is meant to do
   and never ever crashes or reports false results. It has a minimum of 90% testing code
   coverage. That's the topmost goal.

2. Provide appealing C++ software, i.e. software which is **easily read and understood**.
   Second goal.

3. Provide software with **super minimal public interfaces**. I try to really provide the absolute
   minimum without any bloat. The users of the software should only include the main header file
   (e.g. `#include <headcode/logger/logger.hpp>`) and nothing else. These are self-contained and
   rely only on C++ standard headers. No additional 3rd party headers required. Third goal.

4. Provide libraries and binaries with **little to no runtime dependencies**. Yet, linkage might
   require additional libraries (sometimes from `headcode.space` sometimes else). Whenever
   possible I strive to go for the *static libraries*, because a) the resulting binaries when
   linked against these libraries have little to no runtime dependency and can be installed
   directly with a mere `copy` and b) they are smaller too, since the linkers tend to strip off
   stuff which is not needed.

5. In reverse, using and `headcode.space` library or software should not impose and intrusive
   dependencies on this particular software. It should be fairly easy to replace this software
   with something else.

6. Be performant. Yes, speed is impressive too.

I'm by no means perfect. There's always room for improvements and there are sure still bugs.
If you have any suggestions please drop in an email at https://gitlab.com/headcode.space/memtool/-/issues.

SonarQube instance for hcs-mem: https://sonar.ddns.headcode.space/dashboard?id=hcs-mem.


## Project layout

```
.
├── cmake                       CMake additional files (git submodule).
├── include                     Public header files. Add this folder to your C++ search path.
│   └── headcode                
│       └── mem                 Here is the main include: <headcode/mem/mem.hpp>
├── test                        Tests.
│   ├── benchmark               Benchmark tests.
│   ├── shared                  Shared input data files for tests.
│   └── unit                    Unit tests.
├── tools                       Various tools for run-time or build-time.
│   ├── conan                   Conan package manager files.
│   ├── docker                  Dockerfiles for various platforms to build.
│   └── package                 Package related files.
├── Changes.md                  Changes file.
├── CMakeLists.txt              The overall CMakeLists.txt.
├── conanfile.txt               Conan package file.
├── Doxyfile                    Doxgen API documentation configuration.
├── LICENSE.txt                 The software license.
└── README.md                   This file.
```

## Installation

I provide binary installation packages for some operating systems
[here](https://gitlab.com/headcode.space/memtool/-/packages).


## Build

### Dependencies

- cmake
- gcc (with g++) or clang (witch clang++)
- git
- make
- doxygen (with graphviz)
- [conan](https://conan.io) (Conan package manger)
- optional: ninja-build (as an alternative to make)

When cloning this project execute the following to clone submodules as well:

```bash
$ git submodule init
$ git submodule update
```

or simply clone with the `--recurse-submodule` option:
```bash
$ git clone --recurse-submodules
```

You may collect and install all dependencies on your own or use the [conan](https://conan.io) system.
For the latter setup conan (initial one-time; skip this if you have prepared conan locally already)
to bind to libstdc++11 ABI and add the official GitLab.com as remote:
```bash
$ conan profile new default --detect
$ conan profile update settings.compiler.libcxx=libstdc++11 default
$ conan remote add gitlab https://gitlab.com/api/v4/packages/conan
```


#### Native build

mem is a [cmake](https://cmake.org) project with out-of-source builds in
a dedicated folder, usually labeled "build". It also uses conan for dependency 
resolution.

Setup conan (initial one-time; skip this if you have prepared conan locally already):
```bash
$ ( mkdir conan &> /dev/null; cd conan && conan install .. )
```

Prepare the build folder:
```bash
$ mkdir build && cd build
$ cmake ..
$ make
```
or with `ninja` installed:
```bash
$ mkdir build && cd build
$ cmake -GNinja ..
$ ninja
```


#### Docker build images

The `tools/docker` folder contains Dockerfiles along with necessary software
to be installed into the docker containers to create docker builder containers.
These docker containers should be capable to build the software.

The build target `docker_images` will build these images, provided the docker
command is found on the system and the `SHELL` environment variable points to some
POSIX like shell (bash, sh, zsh, ...). The variable `DOCKER_TAG` will be used as
docker container tags.

Example:
```bash
$ cd build
$ cmake -GNinja -D DOCKER_TAG=foo_builder
...
$ ninja docker_images
...
$ docker images | grep hcs-benchmark
REPOSITORY            TAG             IMAGE ID       CREATED         SIZE
foo                   debian-buster   5db480e2bcd8   2 minutes ago   810MB
foo                   fedora32        7f87de7b5e7b   2 minutes ago   1.48GB
foo                   ubuntu-focal    64b74231cd90   2 minutes ago   885MB 
```

Then launch a docker builder like this:
```bash
$ docker run -it --rm --name foo-builder foo:debian-buster /bin/bash
root@d192869e6fe6:/build#
```

In second terminal copy all sources into the container and run a "native" build
there:
```bash
$ cd PROJECT-SOURCES
$ docker cp . foo-builder:/build
``` 


## Test

After compilation run ctest
```bash
$ cd build
$ ctest
```
or
```bash
$ cd build
$ make test
```
or with `ninja` installed:
```bash
$ cd build
$ ninja test
```

_Note: Please check the [test files](test/unit/) for documentation.
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
or with `ninja` installed:
```bash
$ ninja test
$ ninja run-gcovr
```

This will give you the test coverage on stdout as well as:
* `gcovr-coverage.info`:  this is the coverage info file created by gcovr
* `gcovr-report.xml`: this is the gcovr report file in xml
* `coverge-html`: this is the folder in which detailed html info of collected coverage resides
  (open up the file `coverage-html/index.html` in a browser of your choice)
* `gcovr-sonarqube-report.xml`: the same as the `gcov-report.xml` but sprinkled with enough
  fairy dust to make SonarQube swallow it.

in the build folder.


## Installable system package creation

This project supports the creation of `DEB` and `RPM` files. This is done by specifying
the `CPACK_GENERATOR` while configuring the project.

To create an installable `DEB`:
```bash
$ cd build
$ cmake -D CMAKE_BUILD_TYPE=Release -D CPACK_GENERATOR=DEB ..
...
$ make
...
$ make package
```
(or use `ninja` in place of `make` if you use the Ninja generator)

To create an installable `RPM`:
```bash
$ cd build
$ cmake -D CMAKE_BUILD_TYPE=Release -D CPACK_GENERATOR=RPM ..
...
$ make
...
$ make package
```
(or use `ninja` in place of `make` if you use the Ninja generator)


## Conan packages

To create and install conan packages locally, call
```bash
$ cd build
$ make conan 
```

You may want to tweak the package labeling by setting `CONAN_USER` and `CONAN_CHANNEL` arguments
in cmake call prior like this:
```bash
$ cd build
$ cmake -D CONAN_USER=${USER} -D CONAN_CHANNEL="testing" ..
$ make conan 
```


## Notable guidelines

* Coding Guidelines: https://google.github.io/styleguide/cppguide.html
* How (not) to write git commit messages: https://www.codelord.net/2015/03/16/bad-commit-messages-hall-of-shame/
* How to version your software: https://semver.org/
* How to write a clever "Changes" file: https://keepachangelog.com/en/1.0.0/
* Folder Convention: https://github.com/KriaSoft/Folder-Structure-Conventions


---


Copyright (C) 2020-2021 headcode.space e.U.  
Oliver Maurhart <info@headcode.space>  
[https://headcode.space](https://www.headcode.space)  
