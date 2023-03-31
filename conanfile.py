# This is not compatible with Conan v2.x
# Please use v1.x
# If you want v2.x, just change conans to conan.
#
# Note:
# I currently can't upgrade to conan v2 since Qt
# is still not ported to v2
from conans import ConanFile

class Conan(ConanFile):
   name = "spirit"
   version = "0.2.1"
   settings = "os", "compiler", "build_type", "arch"
   generators = ["cmake_find_package", "cmake", "json"]

   def configure(self):
      self.requires("qarchive/2.2.4")
      self.requires("qt/5.15.8")
      self.requires("civetweb/1.15")

      self.options["qarchive"].shared = True
      self.options["civetweb"].shared = True
      self.options["civetweb"].with_ssl = False
      self.options["civetweb"].with_cxx = True
      self.options["qt"].shared = True
      self.options["qt"].qttools = True
      self.options["qt"].qtmultimedia = True
      self.options["qt"].qtimageformats = True
