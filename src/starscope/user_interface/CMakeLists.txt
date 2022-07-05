set(_UI_PREFIX ${_PROJECT_NAME}_user_interface)
set(_STARSCOPE_UI_DISPLAY_NAME ${_UI_PREFIX}_display)

#=================================
# DRIVER DEPENDENCIES
#=================================
include(${CMAKE_CURRENT_LIST_DIR}/../drivers/CMakeLists.txt)

#=================================
# USER INTERFACE DISPLAY
#=================================
add_library(${_STARSCOPE_UI_DISPLAY_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/display.cpp
)

target_include_directories(${_STARSCOPE_UI_DISPLAY_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/lib
)


target_link_libraries(${_STARSCOPE_UI_DISPLAY_NAME} PUBLIC
    ${_DRIVERS_UTILS_NAME}
    ${_DRIVERS_SHARPMEMDISPLAY_NAME}
)
