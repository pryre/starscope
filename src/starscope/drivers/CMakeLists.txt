include_guard()

set(_DRIVERS_PREFIX ${_PROJECT_NAME}_drivers)
set(_DRIVERS_UTILS_NAME ${_DRIVERS_PREFIX}_utils)
set(_DRIVERS_CLOCK_NAME ${_DRIVERS_PREFIX}_clock)
set(_DRIVERS_MPU6050_NAME ${_DRIVERS_PREFIX}_mpu6050)
set(_DRIVERS_SHARPMEMDISPLAY_NAME ${_DRIVERS_PREFIX}_sharpmemdisplay)

#=================================
# DRIVER CLOCK
#=================================
add_library(${_DRIVERS_CLOCK_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/clock.cpp
)

target_include_directories(${_DRIVERS_CLOCK_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${_DRIVERS_CLOCK_NAME} PUBLIC
)

#=================================
# DRIVER UTILS
#=================================
add_library(${_DRIVERS_UTILS_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/utils.cpp
)

target_include_directories(${_DRIVERS_UTILS_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${_DRIVERS_UTILS_NAME} PUBLIC
    ${_DRIVERS_CLOCK_NAME}
)

#=================================
# DRIVER MPU6050
#=================================
add_library(${_DRIVERS_MPU6050_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/mpu6050.cpp
)

target_include_directories(${_DRIVERS_MPU6050_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${_DRIVERS_MPU6050_NAME} PUBLIC
    ${_DRIVERS_UTILS_NAME}
)

#=================================
# DRIVER SHARPMEMDISPLAY
#=================================
add_library(${_DRIVERS_SHARPMEMDISPLAY_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/sharp_mem_display.cpp
)

target_include_directories(${_DRIVERS_SHARPMEMDISPLAY_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${_DRIVERS_SHARPMEMDISPLAY_NAME} PUBLIC
    ${_DRIVERS_UTILS_NAME}
)
