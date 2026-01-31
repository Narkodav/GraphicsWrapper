set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS ON CACHE BOOL "" FORCE)
cmake_minimum_required(VERSION 3.10)
project(GraphicsWrapper)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRC_FILES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/src/**/*.cpp
    ${CMAKE_SOURCE_DIR}/src/**/**/*.cpp
)

# Add executable
add_library(${PROJECT_NAME} STATIC
    ${SRC_FILES}
)

add_library(GraphicsWrapper::GraphicsWrapper ALIAS GraphicsWrapper)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -Wpedantic -Werror
    )
elseif (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4 /WX
    )
endif()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

# Dependencies
set(API_DIR E:/Program\ Files\ \(x86\)/API)
set(LOCAL_API_DIR E:/Program\ Files\ \(x86\)/Code/C_code/libraries)
message(STATUS "Api directory: ${API_DIR}")

# Vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME}
    PUBLIC Vulkan::Vulkan
)

target_include_directories(${PROJECT_NAME}
    SYSTEM PUBLIC ${Vulkan_INCLUDE_DIRS}
)

# GLM
set(glm_DIR ${API_DIR}/glm/cmake/glm)
find_package(glm REQUIRED)
target_link_libraries(${PROJECT_NAME}
    PUBLIC glm::glm
)

target_include_directories(${PROJECT_NAME}
    SYSTEM PUBLIC ${GLM_INCLUDE_DIRS}
)

#STBI
set(stb_image_DIR ${API_DIR}/stb_image)
add_library(stb_image STATIC
    ${stb_image_DIR}/stb_image.cpp
    ${stb_image_DIR}/stb_image_write.cpp
)

add_library(GraphicsWrapper::stb_image ALIAS stb_image)

target_include_directories(stb_image SYSTEM PUBLIC
    ${stb_image_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE stb_image
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(stb_image PRIVATE
        -Wno-error
        -Wno-missing-field-initializers
    )
elseif (MSVC)
    target_compile_options(stb_image PRIVATE
        /W0
    )
endif()

#CommonApi
find_package(CommonApi CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME}
    PUBLIC CommonApi::CommonApi
)
get_target_property(CommonApi_INCLUDE_DIRS CommonApi::CommonApi INTERFACE_INCLUDE_DIRECTORIES)

#Containers
find_package(Containers REQUIRED)
target_link_libraries(${PROJECT_NAME} 
    PUBLIC Containers::Containers
)
get_target_property(Containers_INCLUDE_DIRS Containers::Containers INTERFACE_INCLUDE_DIRECTORIES)

message(STATUS "Vulkan include dirs: ${Vulkan_INCLUDE_DIRS}")
message(STATUS "GLM include dirs: ${GLM_INCLUDE_DIRS}")
message(STATUS "STBI include dirs: ${stb_image_DIR}")
message(STATUS "CommonApi include dirs: ${CommonApi_INCLUDE_DIRS}")
message(STATUS "Containers include dirs: ${Containers_INCLUDE_DIRS}")

# install
install(TARGETS ${PROJECT_NAME}
    EXPORT GraphicsWrapperTargets
)

install(TARGETS stb_image
    EXPORT GraphicsWrapperTargets
    ARCHIVE DESTINATION lib
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
    ${CMAKE_SOURCE_DIR}/install/GraphicsWrapperConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/GraphicsWrapperConfig.cmake
    INSTALL_DESTINATION lib/cmake/GraphicsWrapper
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/GraphicsWrapperConfig.cmake
    DESTINATION lib/cmake/GraphicsWrapper
)
