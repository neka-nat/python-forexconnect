
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

execute_process (
  COMMAND python -c "import site; print(site.getsitepackages()[0])"
  OUTPUT_VARIABLE INSTALL_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
install(TARGETS forexconnect LIBRARY DESTINATION ${INSTALL_DIR})