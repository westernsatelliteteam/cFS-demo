project(CFE_RPI_LIB C)

# Create the app module
if($ENV{SIMULATION} MATCHES "native")
  message(STATUS "RPI Lib: using mocked functions")
  add_cfe_app(rpi_lib fsw/src/rpi_mock.c)
else()
  message(STATUS "RPI Lib: using target functions")
  add_cfe_app(rpi_lib fsw/src/rpi_lib.c)
endif()

# The API to this library (which may be invoked/referenced from other apps)
# is stored in fsw/public_inc.  Using "target_include_directories" is the 
# preferred method of indicating this (vs. directory-scope "include_directories").
target_include_directories(rpi_lib PUBLIC fsw/public_inc)

if (ENABLE_UNIT_TESTS)
  add_subdirectory(ut-stubs)
  add_subdirectory(unit-test)
endif (ENABLE_UNIT_TESTS)


