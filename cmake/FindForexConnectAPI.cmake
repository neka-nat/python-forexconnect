include(FindPackageHandleStandardArgs)

find_path(FOREXCONNECT_INCLUDE_DIR ForexConnect.h
  PATHS
  ${FOREXCONNECT_DIR}/include
  $ENV{FOREXCONNECT_ROOT}/include)

find_library(FOREXCONNECT_LIBRARIES
  NAMES
  ForexConnect
  fxmsg
  fxtp
  gsexpat
  gstool
  httplib
  log4cplus
  pdas
  PATHS
  ${FOREXCONNECT_DIR}/lib
  $ENV{FOREXCONNECT_ROOT}/lib)

find_package_handle_standard_args(ForexConnectAPI DEFAULT_MSG FOREXCONNECT_INCLUDE_DIR FOREXCONNECT_LIBRARIES)
mark_as_advanced(FOREXCONNECT_LIBRARIES FOREXCONNECT_INCLUDE_DIR)
