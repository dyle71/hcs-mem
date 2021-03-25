import os

from conans import ConanFile, tools


class HcsBenchmarkConan(ConanFile):
    name = "hcs-mem"
    version = "1.1.3"
    license = "MIT"
    author = "Oliver Maurhart (oliver.maurhart@headcode.space)"
    url = "https://gitlab.com/headcode.space/memtool"
    homepage = "https://gitlab.com/headcode.space/memtool"
    description = "Very small memory handling and display tool in C++."
    topics = ("memory", "dump", "stream", "C++17")
    no_copy_source = True
    # No settings/options are necessary, this is header only

    def source(self):
        self.run("git clone https://gitlab.com/headcode.space/memtool.git")

    def package(self):
        self.copy("*.hpp", src="mem/include", dst="include")

    def package_id(self):
        self.info.header_only()
