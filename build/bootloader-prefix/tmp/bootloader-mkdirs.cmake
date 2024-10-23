# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/tarik/esp/esp-idf/components/bootloader/subproject"
  "/home/tarik/Doz/build/bootloader"
  "/home/tarik/Doz/build/bootloader-prefix"
  "/home/tarik/Doz/build/bootloader-prefix/tmp"
  "/home/tarik/Doz/build/bootloader-prefix/src/bootloader-stamp"
  "/home/tarik/Doz/build/bootloader-prefix/src"
  "/home/tarik/Doz/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/tarik/Doz/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/tarik/Doz/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
