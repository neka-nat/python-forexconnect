
set(SRCS src/pyforexconnect.cpp
  src/ForexConnectClient.cpp
  src/ResponseListener.cpp)
add_library(forexconnect SHARED ${SRCS})
target_link_libraries(forexconnect
  sample_tools
  ${FOREXCONNECT_LIBRARIES}
  ${Boost_LIBRARIES}
  ${PYTHON_LIBLARY})
set_target_properties(forexconnect PROPERTIES PREFIX "")

install(TARGETS forexconnect LIBRARY DESTINATION ${INSTALL_PYTHON_PKG_DIR})