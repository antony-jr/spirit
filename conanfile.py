from conans import ConanFile, CMake

class Conan(ConanFile):
   name = "spirit"
   version = "0.2.0"
   settings = "arch"
   generators = ["cmake_find_package", "cmake", "json"]

   def configure(self):
      self.requires("qarchive/2.1.1@local/packages")
      self.requires("qt/5.15.3")
      self.requires("civetweb/1.15")

      self.options["qarchive"].shared = True
      self.options["civetweb"].shared = True
      self.options["civetweb"].with_ssl = False
      self.options["civetweb"].with_cxx = True
      self.options["qt"].shared = True
      self.options["qt"].qttools = True
      self.options["qt"].qtmultimedia = True
      self.options["qt"].qtimageformats = True
