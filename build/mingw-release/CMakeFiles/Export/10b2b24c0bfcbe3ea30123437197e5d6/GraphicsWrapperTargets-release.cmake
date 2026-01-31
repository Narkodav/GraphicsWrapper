#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "GraphicsWrapper::GraphicsWrapper" for configuration "Release"
set_property(TARGET GraphicsWrapper::GraphicsWrapper APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(GraphicsWrapper::GraphicsWrapper PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libGraphicsWrapper.a"
  )

list(APPEND _cmake_import_check_targets GraphicsWrapper::GraphicsWrapper )
list(APPEND _cmake_import_check_files_for_GraphicsWrapper::GraphicsWrapper "${_IMPORT_PREFIX}/lib/libGraphicsWrapper.a" )

# Import target "GraphicsWrapper::stb_image" for configuration "Release"
set_property(TARGET GraphicsWrapper::stb_image APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(GraphicsWrapper::stb_image PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libstb_image.a"
  )

list(APPEND _cmake_import_check_targets GraphicsWrapper::stb_image )
list(APPEND _cmake_import_check_files_for_GraphicsWrapper::stb_image "${_IMPORT_PREFIX}/lib/libstb_image.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
