set(_DRIVERS_NAME ${_PROJECT_NAME}_drivers)
set(_DRIVERS_UTILS_NAME ${_DRIVERS_NAME}_utils)
set(_DRIVERS_CLOCK_NAME ${_DRIVERS_NAME}_clock)
set(_DRIVERS_MPU6050_NAME ${_DRIVERS_NAME}_mpu6050)
set(_DRIVERS_SHARPMEMDISPLAY_NAME ${_DRIVERS_NAME}_sharpmemdisplay)

add_library(${_DRIVERS_CLOCK_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/clock.cpp
)

target_include_directories(${_DRIVERS_CLOCK_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${_DRIVERS_CLOCK_NAME} PRIVATE
    pico_stdlib
)

add_library(${_DRIVERS_UTILS_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/utils.cpp
)

target_include_directories(${_DRIVERS_UTILS_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${_DRIVERS_UTILS_NAME} PRIVATE
    ${_DRIVERS_CLOCK_NAME}
)

add_library(${_DRIVERS_MPU6050_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/mpu6050.cpp
)

target_include_directories(${_DRIVERS_MPU6050_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${_DRIVERS_MPU6050_NAME} PRIVATE
    pico_stdlib
    hardware_i2c
    ${_DRIVERS_UTILS_NAME}
)

add_library(${_DRIVERS_SHARPMEMDISPLAY_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/sharp_mem_display.cpp
)

target_include_directories(${_DRIVERS_SHARPMEMDISPLAY_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${_DRIVERS_SHARPMEMDISPLAY_NAME} PRIVATE
    pico_stdlib
    hardware_spi
    ${_DRIVERS_UTILS_NAME}
)
