set(DEFAULT_FOREX_URL "http://www40.fxcorporate.com/Hosts.jsp" CACHE STRING "Forexconnect url")
configure_file(src/ForexConnectClient.h.in ${CMAKE_CURRENT_BINARY_DIR}/src/ForexConnectClient.h)
include_directories(src ${CMAKE_CURRENT_BINARY_DIR}/src)
set(SRCS src/pyforexconnect.cpp
  src/ForexConnectClient.cpp
  src/SessionStatusListener.cpp
  src/ResponseListener.cpp)
add_library(forexconnect SHARED ${SRCS})
target_link_libraries(forexconnect
  sample_tools
  ${FOREXCONNECT_LIBRARIES}
  ${Boost_LIBRARIES}
  ${PYTHON_LIBLARY})
set_target_properties(forexconnect PROPERTIES PREFIX "")

add_executable(login cpp_sample/Login.cpp src/SessionStatusListener.cpp)
target_link_libraries(login
  sample_tools
  ${FOREXCONNECT_LIBRARIES}
  ${Boost_LIBRARIES})

install(TARGETS forexconnect LIBRARY DESTINATION ${INSTALL_PYTHON_PKG_DIR})