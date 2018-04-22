cmake_minimum_required(VERSION 3.10)

# General
include("../cmake/BundleQt.cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#find_package(pybind11 CONFIG REQUIRED)
include("../cmake/pybind.cmake")


find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5QuickControls2 REQUIRED)


# NumPy
find_path(NUMPY_DIR NAMES "core/include/numpy/ndarrayobject.h" PATHS "${PYTHON_SITE_PACKAGES}/numpy"  NO_DEFAULT_PATH DOC "Path to NumPy")
set(NUMPY_INCLUDE_DIR "${NUMPY_DIR}/core/include")
set(NUMPY_LIBRARY_DIR "${NUMPY_DIR}/core/lib")
set(NUMPY_LIBRARIES npymath)
set(NUMPY_DEFINITIONS -DNPY_NO_DEPRECATED_API=NPY_API_VERSION)


if (WIN32)
  add_definitions(-DWIN32_LEAN_AND_MEAN)
endif()

set(SOURCES
    qui.cpp
    Types.cpp
    QtTypes.cpp
    QMLTypes.cpp
    PaintTypes.cpp
    Connection.cpp
    Method.cpp
    Items.cpp
    Utility.cpp
    )

set(HEADERS
    qui.h
    Types.h
    BaseTypes.h
    QtTypes.h
    QtSimpleTypes.h
    ScalarTypes.h
    Holder.h
    NDArray.h
    Sequence.h
    Connection.h
    Method.h
    Items.h
    Utility.h)


link_directories(${NUMPY_LIBRARY_DIR})

#add_library(qui MODULE ${SOURCES} ${HEADERS})
#set_target_properties(qui PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}" SUFFIX "${PYTHON_MODULE_EXTENSION}" OUTPUT_NAME "qui_")
pybind11_add_module(qui MODULE ${SOURCES} ${HEADERS})
set_target_properties(qui PROPERTIES OUTPUT_NAME "qui_")
target_compile_definitions(qui PRIVATE ${NUMPY_DEFINITIONS} )
target_include_directories(qui PRIVATE ${PYTHON_INCLUDE_DIR} ${NUMPY_INCLUDE_DIR} ${pybind11_INCLUDE_DIR})
target_link_libraries(qui PRIVATE ${NUMPY_LIBRARIES} pybind11::module Qt5::Quick Qt5::Widgets Qt5::QuickControls2)


install(TARGETS qui LIBRARY DESTINATION "qui")


if (WIN32)
  set(BUNDLE_QT TRUE CACHE BOOL "Bundle Qt")
  if (${BUNDLE_QT})
    bundle_qt()
  endif()
endif()
