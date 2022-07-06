set(_NODE_NAME ${_PROJECT_NAME}_pico)
list(APPEND _NODE_LIBRARIES "pico_stdlib")

#=================================
# PICO SDK
#=================================

# Pull in SDK (must be before project)
set(ENV{PICO_SDK_PATH} /usr/share/pico-sdk)
# set(ENV{PICO_SDK_FETCH_FROM_GIT} FALSE)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# Initialize the SDK
pico_sdk_init()

# include(example_auto_set_url.cmake)


#=================================
# DRIVER DEPENDENCIES
#=================================
include(${CMAKE_CURRENT_LIST_DIR}/drivers/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/user_interface/CMakeLists.txt)

add_executable(${_NODE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/${_NODE_NAME}.cpp
)

target_include_directories(${_NODE_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# pull in common dependencies and additional i2c hardware support
target_link_libraries(${_NODE_NAME} PUBLIC
    pico_stdlib
    hardware_i2c
    hardware_spi
    ${_DRIVERS_UTILS_NAME}
    ${_DRIVERS_MPU6050_NAME}
    ${_STARSCOPE_UI_DISPLAY_NAME}
)

if (TARGET tinyusb_device)
        # enable usb output, disable uart output
        pico_enable_stdio_usb(${_NODE_NAME} 1)
        pico_enable_stdio_uart(${_NODE_NAME} 0)
        # target_compile_definitions(${_NODE_NAME} PRIVATE
        #     PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS=5000
        # )
elseif(PICO_ON_DEVICE)
    message(WARNING "using uart output because TinyUSB submodule is not initialized in the SDK")
endif()

# create map/bin/hex file etc.
pico_add_extra_outputs(${_NODE_NAME})

# add url via pico_set_program_url
# example_auto_set_url(i2c_bus_scan)

