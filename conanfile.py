from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake
from conan.tools.files import copy
import os

class QuetzalCoaTLConan(ConanFile):

    name            = "quetzal-CoaTL"
    url             = "https://github.com/Quetzal-framework/quetzal-CoaTL"
    license         = "GPLv3"
    description     = "Coalescence library for C++"
    version         = "3.1.0"

    settings        = "os", "compiler", "arch", "build_type"
    exports_sources = "src/*", "CMakeLists.txt", "test/*", "cmake/*", "docs/*"
    no_copy_source  = True
    build_policy    = "missing"
    
    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()

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
        self.requires("boost/1.83.0")