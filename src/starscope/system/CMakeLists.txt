include_guard()

set(_UI_PREFIX ${_PROJECT_NAME}_system)
set(_STARSCOPE_SYSTEM_STATE_NAME ${_UI_PREFIX}_state)

#=================================
# DRIVER DEPENDENCIES
#=================================
include(${CMAKE_CURRENT_LIST_DIR}/../drivers/CMakeLists.txt)

#=================================
# USER INTERFACE DISPLAY
#=================================
add_library(${_STARSCOPE_SYSTEM_STATE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/state.cpp
)

target_include_directories(${_STARSCOPE_SYSTEM_STATE_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)


target_link_libraries(${_STARSCOPE_SYSTEM_STATE_NAME} PUBLIC
)
