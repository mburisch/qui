

function(bundle_qt)
  file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/qt_files")
  set(WINDEPLOYQT "${Qt5Core_DIR}/../../../bin/windeployqt.exe")
  set(WINDEPLOYQT_ARGS
      "--dir" "${CMAKE_BINARY_DIR}/qt_files"
      "--no-compiler-runtime"
      "--no-system-d3d-compiler"
      "--no-webkit2"
      "--no-opengl-sw"
      "--release"
      "--plugindir" "${CMAKE_BINARY_DIR}/qt_files/plugins"
      "--libdir" "${CMAKE_BINARY_DIR}/qt_files/bin"
      "$<TARGET_FILE:qui>")

  add_custom_command(TARGET qui
                     POST_BUILD
                     COMMAND ${WINDEPLOYQT} ARGS ${WINDEPLOYQT_ARGS}
                     WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/qt_files"
                     USES_TERMINAL)

  install(DIRECTORY "${CMAKE_BINARY_DIR}/qt_files/" DESTINATION  "qui/Qt")
  install(DIRECTORY "${Qt5Core_DIR}/../../../qml/" DESTINATION  "qui/Qt/qml" PATTERN "*.pdb" EXCLUDE)

endfunction()


