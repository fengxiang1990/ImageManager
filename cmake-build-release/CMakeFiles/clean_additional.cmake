# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\ImageManager_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\ImageManager_autogen.dir\\ParseCache.txt"
  "ImageManager_autogen"
  )
endif()
