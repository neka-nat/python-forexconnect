cmake_minimum_required(VERSION 2.8) 

find_package(Boost COMPONENTS log date_time python REQUIRED)
find_package(PythonLibs REQUIRED)
set(FOREXCONNECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/forexconnect")
set(SAMPLE_TOOLS_DIR "${FOREXCONNECT_DIR}/sample_tools")
add_definitions(-DBOOST_LOG_DYN_LINK)
include_directories(${Boost_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_DIR}
  ${FOREXCONNECT_DIR}/api/include
  ${SAMPLE_TOOLS_DIR}/include)   

link_directories(${FOREXCONNECT_DIR}/api/lib)

add_subdirectory(${SAMPLE_TOOLS_DIR})
add_subdirectory(forexconnect)