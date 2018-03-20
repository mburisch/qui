message("Bundling Qt")

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/qt_files")

if (WIN32)
  file(GLOB QUI_DLL "${CMAKE_INSTALL_PREFIX}/qui/qui_.*")
  find_program(TOOL_WINDEPLOYQT NAMES windeployqt PATHS "${CMAKE_PREFIX_PATH}")
  execute_process(COMMAND ${TOOL_WINDEPLOYQT}
                  "--dir" "${CMAKE_BINARY_DIR}/qt_files"
                  "--no-compiler-runtime"
                  "--no-system-d3d-compiler"
                  "--no-webkit2"
                  "--no-opengl-sw"
                  "--no-plugins"
                  "--no-translations"
                  "--release"
                  "${QUI_DLL}" 
                  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/qt_files")
endif()

