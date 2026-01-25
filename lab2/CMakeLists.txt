# 
# Generally starts every CMakeLists.txt file.  Setting Cmake, C and C++ versions
#
cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

#
# For CS466 my labs and documentation expect all class material to be based off a 
# single directory.  In my case thei is ${HOME}/src You are free to change this to match 
# your environment.
#
set(MY_ROOT_DIR $ENV{HOME}/src)
message(STATUS "Using project ROOT dir = ${MY_ROOT_DIR}")

#
# An option to select the Pico-2 board (C33 core) vs the original Pico board (M0+ core)  
#
option(ENABLE_PICO_2 "Set ON tocompile for Pico2 (C33 core) otherwise set OFF for orginal Pico (M0+ core)" OFF)

if(ENABLE_PICO_2)
    set(PICO_BOARD pico2)
    set(PICO_PLATFORM rp2350-arm-s)
    set(destination_file "/media/$ENV{USER}/RP2350")
    set(FREERTOS_PORT_DIR portable/GCC/ARM_CM33)
    set(FREERTOS_PORT GCC_ARM_CM33)
else()
    set(PICO_BOARD pico)        # or pico_w / pico2 / etc.
    set(PICO_PLATFORM rp2040)   # or rp2350 if using Pico 2
    set(destination_file "/media/$ENV{USER}/RPI-RP2")
    set(FREERTOS_PORT_DIR portable/ThirdParty/GCC/RP2040)
    set(FREERTOS_PORT GCC_ARM_CM0)
endif()

# Enable stdio over USB + UART (very common choice)
#set(PICO_DEFAULT_BINARY_INFO 1)
#set(PICO_STDIO_USB 1)
#set(PICO_STDIO_UART 0)      # Provides Serial on Board Pins 1 and 2
#
# The pico build tools are a liuttle odd but make for a better cmake file below.
# generally cross-compile cmake systems that I've used int he past use a tools file 
# as part of the command line.. (.e.g cmake -DCMAKE_TOOLCHAIN_FILE="../armgcc.cmake" ) 
# but the cmake foolery in this included file sets up most of the environment.
#
include(${MY_ROOT_DIR}/pico/pico-sdk/external/pico_sdk_import.cmake)

#
set(FREERTOS_KERNEL_PATH ${MY_ROOT_DIR}/FreeRTOS-LTS/FreeRTOS/FreeRTOS-Kernel)

# Set the cmake 'Project'.  This line sets up the cmake target and does an implicent define of PROJECT_NAME
# that I use below to not have to put the 'lab2' project on every definition.  
#
project(lab2 C CXX ASM)

# Here we set the build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: Debug Release" FORCE)
endif()

# Set compiler flags for debugging and disabling optimization
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Og -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -g")

# Ensure these flags are also set for release builds, if you want no optimization at all
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Og -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Og -g")

# For the executable, ensure debug flags are explicitly applied
#target_compile_options(${PROJECT_NAME} PRIVATE -Og -g)

#
# a way to make sure that you have a support library version at or greater than a needed library.
#
if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# 
# Initialize the SDK
#
pico_sdk_init()

# Option A: You git submodule'd FreeRTOS into lib/FreeRTOS-Kernel
#           (most common & clean approach)

#add_library(freertos_kernel INTERFACE)

#target_sources(freertos_kernel INTERFACE
#    # Core kernel
#    ${FREERTOS_KERNEL_PATH}/tasks.c
#    ${FREERTOS_KERNEL_PATH}/queue.c
#    ${FREERTOS_KERNEL_PATH}/list.c
#    ${FREERTOS_KERNEL_PATH}/timers.c
#    ${FREERTOS_KERNEL_PATH}/portable/MemMang/heap_4.c   # ← most common choice
#
#    # RP2040 / RP2350 port (choose GCC)
#    ${FREERTOS_KERNEL_PATH}/${FREERTOS_PORT_DIR}/port.c
#)

#target_include_directories(freertos_kernel INTERFACE
#    ${FREERTOS_KERNEL_PATH}/include
#    ${FREERTOS_KERNEL_PATH}/${FREERTOS_PORT_DIR}
#    ${CMAKE_CURRENT_LIST_DIR}/      # ← for your own FreeRTOSConfig.h
#)
#
# I'm not terriably happy with the default options that the Pico folks choose...  I prefer to set warnings as errors then 
# disable specific warnings as theyu have done below.  When you perform the make you can add the command line option 
# VERBOSE=1 and the generated makefile will dump the full command line for each compile and show all the warnings..  There 
# are several that we would fix if this was in my workplace where warniongs count as compile errors.
#
add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        #-Wno-unused-variable # Allow unused variables (I don't generlly like this but FreeRTOS has some)
        #-Wno-array-bounds    # pico libraries really blow this
        )

#if (NOT FREERTOS_KERNEL_PATH AND NOT DEFINED ENV{FREERTOS_KERNEL_PATH})
##    message("Skipping FreeRTOS examples as FREERTOS_KERNEL_PATH not defined")
#    return()
#endif()

include(FreeRTOS_Kernel_import.cmake)

#
# Add the long list of .c files that we have for lab2.
# As we add files for later consideration thys will get added like lab2.c below.
#
add_executable(${PROJECT_NAME}
        lab2.c
        )

#
# pull in common dependencies, This is where cmake adds the pico and freertos libraries to ourt project
#
target_link_libraries(${PROJECT_NAME} 
        pico_stdlib 
#        pico-multicore
        FreeRTOS-Kernel-Heap4  
        hardware_exception)

target_include_directories(${PROJECT_NAME} PRIVATE
    .
#    ${FREERTOS_KERNEL_PATH}/include
#    ${FREERTOS_KERNEL_PATH}/${FREERTOS_PORT_DIR}/include
) 


# 
# tell the pico library that you will be using usb serial and not an actual uart on the 
# processor.  If you have a serial uart connected to a terminal you can enable one or the other. 
# I don't recommend setting both.
#
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# 
# create map/bin/hex file etc.  These are generally extra files that are useful in addition to the .elf file.
#
pico_add_extra_outputs(${PROJECT_NAME})

#
# Use the ARM tools to dump sizes of the major segments in the file.  As your projects get large this is 
# something yoiu want to keep track of.
#
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND arm-none-eabi-size ${PROJECT_NAME}.elf)

# Define the source and destination files. For Pico 2 use RPI2350, otherwise use RPI-RP2
set(source_file "${PROJECT_NAME}.uf2")


# 
# Add a custom command to copy the file
#
# Until you are using a debugger the pico is programmed by copying a .uf2 file to the faux
# usb drive manufactured by the Pico bootloader.  The code below will allow the command line 
# `make flash` to be used.  Be sure to boot the Pico into it's faux-drive mode before running this 
# command
#
add_custom_command(
    OUTPUT ${destination_file}
    COMMAND ${CMAKE_COMMAND} -E copy ${source_file} ${destination_file}
    DEPENDS ${source_file} 
    COMMENT "Copying file: ${source_file} to ${destination_file}"
)

# Add a custom target that depends on the custom command
add_custom_target(flash DEPENDS ${destination_file})