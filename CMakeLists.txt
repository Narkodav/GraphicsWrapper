cmake_minimum_required(VERSION 3.20)
project(GraphicsWrapper LANGUAGES CXX)

# ===============================
# Library target
# ===============================

file(GLOB_RECURSE SRC_FILES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(GraphicsWrapper ${SRC_FILES})
add_library(GraphicsWrapper::GraphicsWrapper ALIAS GraphicsWrapper)

target_compile_features(GraphicsWrapper PUBLIC cxx_std_20)

target_include_directories(GraphicsWrapper
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_options(GraphicsWrapper PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE GRAPHICS_WRAPPER_EXPORTS
)

# ===============================
# System dependency
# ===============================

if(PROJECT_IS_TOP_LEVEL)
    find_package(Vulkan REQUIRED)
endif()
target_link_libraries(GraphicsWrapper PUBLIC Vulkan::Vulkan)

# ===============================
# Internal dependencies (PRIVATE)
# ===============================

add_library(GraphicsWrapper_stb_image INTERFACE)
target_include_directories(GraphicsWrapper_stb_image
    INTERFACE 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Vendor/stb_image>
        $<INSTALL_INTERFACE:include/GraphicsWrapper/stb_image>
)
target_link_libraries(GraphicsWrapper PRIVATE GraphicsWrapper_stb_image)

# ===============================
# Other libraries
# ===============================

if(PROJECT_IS_TOP_LEVEL)
    find_package(CommonApi REQUIRED CONFIG)
    find_package(Containers REQUIRED CONFIG)
endif()
target_link_libraries(GraphicsWrapper PUBLIC CommonApi::CommonApi)
target_link_libraries(GraphicsWrapper PUBLIC Containers::Containers)

# =========================
# Test executable
# =========================
file(GLOB_RECURSE TEST_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp
)

add_executable(GraphicsWrapperTests ${TEST_SRC_FILES})

# Link the library
target_link_libraries(GraphicsWrapperTests PRIVATE ${PROJECT_NAME})

# Match C++ standard and warnings
target_compile_features(GraphicsWrapperTests PUBLIC cxx_std_20)
target_compile_options(GraphicsWrapperTests PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

target_include_directories(GraphicsWrapperTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Enable CTest
enable_testing()
add_test(NAME GraphicsWrapperTests COMMAND GraphicsWrapperTests)

# ===============================
# Install & package (TOP-LEVEL ONLY)
# ===============================

install(TARGETS ${PROJECT_NAME} GraphicsWrapper_stb_image
    EXPORT GraphicsWrapperTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/
    DESTINATION include
)

install(EXPORT GraphicsWrapperTargets
    FILE GraphicsWrapperTargets.cmake
    NAMESPACE GraphicsWrapper::
    DESTINATION lib/cmake/GraphicsWrapper
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/install/GraphicsWrapperConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/GraphicsWrapperConfig.cmake
    INSTALL_DESTINATION lib/cmake/GraphicsWrapper
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/GraphicsWrapperConfig.cmake
    DESTINATION lib/cmake/GraphicsWrapper
)