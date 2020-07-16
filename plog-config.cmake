# plog-config.cmake - package configuration file for plog library
#
# exports:
# 		plog - as INTERFACE library target with its INTERFACE_INCLUDE_DIRECTORIES set
#
# sets:
# 		PLOG_INCLUDE_DIR - the directory containing plog headers



if(NOT TARGET plog)
  include("${CMAKE_CURRENT_LIST_DIR}/plogTargets.cmake")
  get_target_property(PLOG_INCLUDE_DIR plog INTERFACE_INCLUDE_DIRECTORIES)
  add_library(plog::plog INTERFACE IMPORTED GLOBAL)
  set_target_properties(plog::plog PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${PLOG_INCLUDE_DIR})
endif()
