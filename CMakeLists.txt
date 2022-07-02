cmake_minimum_required(VERSION 3.12)
set(_PROJECT_NAME starscope)

# Pull in SDK (must be before project)
set(ENV{PICO_SDK_PATH} /usr/share/pico-sdk)
# set(ENV{PICO_SDK_FETCH_FROM_GIT} FALSE)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(${_PROJECT_NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# Initialize the SDK
pico_sdk_init()

# include(example_auto_set_url.cmake)

# Compiler options
set(CMAKE_DEBUG_POSTFIX d)

add_compile_options(-Wall
        # -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        # -Wno-unused-function # we have some for the docs that aren't called
        # -Wno-maybe-uninitialized
)

include(./src/starscope/CMakeLists.txt)

# add_executable(${_NODE_NAME}
#         ./src/starscope/${_NODE_NAME}.cpp
# )

# target_include_directories(${_NODE_NAME} PRIVATE
#         ./include
# )

# # pull in common dependencies and additional i2c hardware support
# target_link_libraries(${_NODE_NAME} PRIVATE pico_stdlib hardware_i2c)

# if (TARGET tinyusb_device)
#         # enable usb output, disable uart output
#         pico_enable_stdio_usb(${_NODE_NAME} 1)
#         pico_enable_stdio_uart(${_NODE_NAME} 0)
# elseif(PICO_ON_DEVICE)
#     message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
# endif()

# # create map/bin/hex file etc.
# pico_add_extra_outputs(${_NODE_NAME})

# # add url via pico_set_program_url
# # example_auto_set_url(i2c_bus_scan)
