# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles/madlibs_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/madlibs_autogen.dir/ParseCache.txt"
  "CMakeFiles/test_stories_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/test_stories_autogen.dir/ParseCache.txt"
  "madlibs_autogen"
  "test_stories_autogen"
  )
endif()
