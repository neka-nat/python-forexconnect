cmake_minimum_required(VERSION 2.8) 

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(Boost COMPONENTS log date_time python REQUIRED)
find_package(PythonLibs REQUIRED)
find_package(ForexConnectAPI REQUIRED)
set(SAMPLE_TOOLS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/forexconnect/sample_tools")
add_definitions(-DBOOST_LOG_DYN_LINK)
include_directories(${Boost_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_DIR}
  ${FOREXCONNECT_INCLUDE_DIR}
  ${SAMPLE_TOOLS_DIR}/include)   

execute_process (
  COMMAND python -c "import site; print(site.getsitepackages()[0])"
  OUTPUT_VARIABLE INSTALL_PYTHON_PKG_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)

set(INSTALL_TARGETS_DEFAULT_ARGS
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

if (WIN32)
  link_directories("${CMAKE_CURRENT_SOURCE_DIR}/forexconnect/sample_tools/lib/Win32")
else ()
  add_subdirectory(${SAMPLE_TOOLS_DIR})
endif ()
add_subdirectory(forexconnect)