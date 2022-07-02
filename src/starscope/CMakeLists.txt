set(_NODE_NAME ${_PROJECT_NAME}_pico)

include(${CMAKE_CURRENT_LIST_DIR}/drivers/CMakeLists.txt)

add_executable(${_NODE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/${_NODE_NAME}.cpp
)

target_include_directories(${_NODE_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# pull in common dependencies and additional i2c hardware support
target_link_libraries(${_NODE_NAME} PRIVATE
    pico_stdlib
    hardware_i2c
    ${_DRIVERS_UTILS_NAME}
)

if (TARGET tinyusb_device)
        # enable usb output, disable uart output
        pico_enable_stdio_usb(${_NODE_NAME} 1)
        pico_enable_stdio_uart(${_NODE_NAME} 0)
elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()

# create map/bin/hex file etc.
pico_add_extra_outputs(${_NODE_NAME})

# add url via pico_set_program_url
# example_auto_set_url(i2c_bus_scan)
