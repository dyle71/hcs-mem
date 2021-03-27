# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]


## [1.1.5] - 2021-03-27
### Removed
- No need for GoogleTest as submodule


## [1.1.4] - 2021-03-27
### Removed
- No need for top level conan folder any longer


## [1.1.3] - 2021-03-25
### Added
- Support for Ninja builder.
- Support for conan package manager.
### Changed
- Dropped git submodules which can be substituted with conan


## [1.1.2] - 2021-02-04
### Added
- Completed the rule of 5 for `MemoryManipulator`.
- SonarQube support for profiling.
### Changed
- Dropped internal benchmark tool and replaced it with 3rd/ gitlab link.
- Enforces some more strict type conversion.


## [1.1.1] - 2020-11-18
### Added
- Added headcode::mem::MemoryToHex(char const * memory, std::uint64_t size) function.


## [1.1.0] - 2020-11-16
### Changed
- Renamed whole project from 'memtool' to simply 'mem'


## [1.0.3] - 2020-11-13
### Changed
- Improved performance at canonical output even more
- Canonical output offset is now 64 bit hex value


## [1.0.2] - 2020-11-12
### Added
- Added canonical output also for old-school memory (char * and size)
### Changed
- Improved performance at canonical output

## [1.0.1] - 2020-09-10
### Changed
- Improved performance at byte to hex conversion
- Use hard-coded version numbers (as induced in a header only solution)

## [1.0.0] - 2020-09-09
### Added
- Initial release.


[Unreleased]: https://gitlab.com/headcode.space/mem/-/tree/develop
[1.1.5]: https://gitlab.com/headcode.space/memtool/-/releases/v1.1.5
[1.1.4]: https://gitlab.com/headcode.space/memtool/-/releases/v1.1.4
[1.1.3]: https://gitlab.com/headcode.space/memtool/-/releases/v1.1.3
[1.1.2]: https://gitlab.com/headcode.space/memtool/-/releases/v1.1.2
[1.1.1]: https://gitlab.com/headcode.space/memtool/-/releases/v1.1.1
[1.1.0]: https://gitlab.com/headcode.space/memtool/-/releases/v1.1.0
[1.0.3]: https://gitlab.com/headcode.space/memtool/-/releases/v1.0.3
[1.0.2]: https://gitlab.com/headcode.space/memtool/-/releases/v1.0.2
[1.0.1]: https://gitlab.com/headcode.space/memtool/-/releases/v1.0.1
[1.0.0]: https://gitlab.com/headcode.space/memtool/-/releases/v1.0.0


---


Copyright (C) 2020-2021 headcode.space e.U.  
Oliver Maurhart <info@headcode.space>  
[https://headcode.space](https://www.headcode.space)  
