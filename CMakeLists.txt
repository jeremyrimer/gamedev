cmake_minimum_required(VERSION 3.18)
project(PixelEngine)

set(CMAKE_CXX_STANDARD 17)

# Set to your real SDL3 base directory
set(SDL3_ROOT "C:/programming/libs/SDL3-3.2.16/x86_64-w64-mingw32")
set(SDL3_IMAGE_ROOT "C:/programming/libs/SDL_image")
set(SDL3_TTF_ROOT "C:/programming/libs/SDL_ttf")
set(INSTALL_DIR "C:/programming/libs/install")

# Add include and lib paths
link_directories(${SDL3_ROOT}/lib)
link_directories(${SDL3_IMAGE_ROOT}/build)
link_directories(${INSTALL_DIR}/lib)

# Define code content
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")
add_executable(PixelEngine ${SRC_FILES})

target_include_directories(PixelEngine PRIVATE 
  "${SDL3_ROOT}/include"
  "${SDL3_IMAGE_ROOT}/include"
  "${SDL3_TTF_ROOT}/include"
  "${PROJECT_SOURCE_DIR}/include"
  "${INSTALL_DIR}/include"
  "${INSTALL_DIR}/include/freetype2"
)

# Link to SDL3
target_link_libraries(PixelEngine PRIVATE SDL3 "${SDL3_IMAGE_ROOT}/build/libSDL3_image.dll.a" SDL3_ttf freetype)

# Copy DLL to build output
add_custom_command(TARGET PixelEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SDL3_ROOT}/bin/SDL3.dll"
    "${SDL3_IMAGE_ROOT}/build/SDL3_image.dll"
    "${INSTALL_DIR}/bin/SDL3_ttf.dll"
    $<TARGET_FILE_DIR:PixelEngine>
)
