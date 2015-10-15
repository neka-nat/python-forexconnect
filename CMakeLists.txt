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

add_subdirectory(${SAMPLE_TOOLS_DIR})
add_subdirectory(forexconnect)