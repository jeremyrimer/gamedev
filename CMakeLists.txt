cmake_minimum_required(VERSION 3.18)
project(PixelEngine)

set(CMAKE_CXX_STANDARD 17)

# Set to your real SDL3 base directory
set(SDL3_ROOT "C:/programming/libs/SDL3-3.2.16/x86_64-w64-mingw32")
set(SDL3_IMAGE_ROOT "C:/programming/libs/SDL_image")


# Add include and lib paths
link_directories(${SDL3_ROOT}/bin)
link_directories(${SDL3_IMAGE_ROOT}/build)

# Define code content
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")
add_executable(PixelEngine ${SRC_FILES})

target_include_directories(PixelEngine PRIVATE 
  "${SDL3_ROOT}/include"
  "${SDL3_IMAGE_ROOT}/include"
)

# Link to SDL3
target_link_libraries(PixelEngine PRIVATE SDL3 SDL3_image)

# Copy DLL to build output
add_custom_command(TARGET PixelEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SDL3_ROOT}/bin/SDL3.dll"
    $<TARGET_FILE_DIR:PixelEngine>
)
