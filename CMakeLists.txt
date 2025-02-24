cmake_minimum_required(VERSION 3.27)
include(CMakePrintHelpers)

project(audio_programming_template)

set(RTMIDI_TARGETNAME_UNINSTALL "rtmidi-uninstall")
set(ENABLE_PRECOMPILED_HEADERS OFF)

option (BUILD_PROGRAMS "Build programs" OFF)
option (BUILD_EXAMPLES "Build examples" OFF)
option (BUILD_TESTING "Build tests" OFF)
option (ENABLE_CPACK "Enable CPack support" OFF)
option(RTMIDI_BUILD_TESTING "Build test programs" OFF)
option(ENABLE_FLOAT "Enable float precision in FFTW3" ON)

add_subdirectory("${PROJECT_SOURCE_DIR}/libsndfile")
add_subdirectory("${PROJECT_SOURCE_DIR}/fftw-3.3.10")
add_subdirectory("${PROJECT_SOURCE_DIR}/portaudio")
add_subdirectory("${PROJECT_SOURCE_DIR}/rtmidi")

set(RTMIDI_TARGETNAME_UNINSTALL "rtmidi-uninstall" CACHE STRING "Name of 'uninstall' build target")

include_directories("${PROJECT_SOURCE_DIR}/fftw-3.3.10/api")
add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} PortAudio SndFile::sndfile rtmidi ${FFTW3_LIBRARIES} fftw3f)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
