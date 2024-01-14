from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake, cmake_layout
from conan.tools.files import copy
import os

required_boost_modules = [
"program_options",
"test",
"container",
"system",
"filesystem",
"atomic",
"exception",
"log",
"date_time",
"random",
"regex",
"thread",
"chrono"
]

boost_modules = [
'math', 'wave', 'container', 'contract', 'exception', 'graph', 'iostreams', 'locale', 'log',
'program_options', 'random', 'regex', 'mpi', 'serialization',
'coroutine', 'fiber', 'context', 'timer', 'thread', 'chrono', 'date_time',
'atomic', 'filesystem', 'system', 'graph_parallel', 'python',
'stacktrace', 'test', 'type_erasure'
]

class ProjectConan(ConanFile):

    name            = "projectL"
    url             = "https://github.com/Meetup-Cpp-Warm-Coffee-in-Paris/1brc-challenge"
    license         = "GPLv3"
    description     = "C++ project"
    version         = "0.1"

    settings        = "os", "compiler", "arch", "build_type"
    exports_sources = "src/*", "CMakeLists.txt", "test/*", "cmake/*", "docs/*"
    no_copy_source  = True
    build_policy    = "missing"
        
    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()

    def layout(self):
        cmake_layout(self)

    def build(self): # this is not building a library, just tests
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.test()

    def package(self):
        copy(self, "*.h*",
            src=os.path.join(self.source_folder, "include"),
            dst=os.path.join(self.package_folder, "include"))

    def package_id(self):
        self.info.clear()

    def requirements(self):
        default_options = { "shared": True }
        default_options.update({"without_%s" % module: True for module in [boost_module for boost_module in boost_modules if boost_module not in required_boost_modules]})
        self.requires("boost/1.79.0", options=default_options)