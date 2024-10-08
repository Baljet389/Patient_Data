# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Patient_Data_Project_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Patient_Data_Project_autogen.dir\\ParseCache.txt"
  "Patient_Data_Project_autogen"
  )
endif()
