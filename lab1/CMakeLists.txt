
cmake_minimum_required(VERSION 3.13)

#
# For CS466 my labs and documentation expect all class material to be based off a 
# single directory.  In my case thei is ${HOME}/src You are free to change this to match 
# your environment.
#
set(MY_ROOT_DIR $ENV{HOME}/src)
message(STATUS "Using project ROOT dir = ${MY_ROOT_DIR}")


#
# The pico build tools are a liuttle odd but make for a better cmake file below.
# generally cross-compile cmake systems that I've used int he past use a tools file 
# as part of the command line.. (.e.g cmake -DCMAKE_TOOLCHAIN_FILE="../armgcc.cmake" ) 
# but the cmake foolery in this included file sets up most of the environment.
#
message(STATUS "Include pico_sdk_import.cmake")
include(${MY_ROOT_DIR}/pico/pico-sdk/external/pico_sdk_import.cmake)

message(STATUS "Includes Complete")
message(STATUS "    gcc = ${CMAKE_C_COMPILER}")
message(STATUS "    g++ = ${CMAKE_CXX_COMPILER}")
message(STATUS "    as = ${CMAKE_ASM_COMPILER}")

project(lab1 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# Initialize the SDK
message(STATUS "Initialize the SDK")
pico_sdk_init()
message(STATUS "Initialize complete")

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )


add_executable(lab1
        lab1.c
        )

# pull in common dependencies
target_link_libraries(lab1 pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(lab1)

# add url via pico_set_program_url
# example_auto_set_url(blink)
