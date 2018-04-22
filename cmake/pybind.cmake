#Either find existing pybind11 or download and use it

find_package(pybind11 CONFIG)

if (NOT ${pybind11_FOUND})
  set(PYBIND_SOURCES "${CMAKE_BINARY_DIR}/pybind/pybind.tar.gz")
  if (NOT EXISTS ${PYBIND_SOURCES})
    file(DOWNLOAD "https://github.com/pybind/pybind11/archive/v2.2.2.tar.gz" "${PYBIND_SOURCES}" SHOW_PROGRESS)
	execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${PYBIND_SOURCES} WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/pybind")
	
  endif()
  add_subdirectory("${CMAKE_BINARY_DIR}/pybind/pybind11-2.2.2" "${CMAKE_BINARY_DIR}/pybind/build")
  find_package(PythonLibs REQUIRED)
  set(PYTHON_SITE_PACKAGES "${PYTHON_INCLUDE_DIR}/../Lib/site-packages")
endif()

