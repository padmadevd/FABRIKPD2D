cmake_minimum_required(VERSION 4.1.1)

set(SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(BUILD_DIR "${SOURCE_DIR}/build")

message(STATUS "Configuring project...")
execute_process(
    COMMAND ${CMAKE_COMMAND} -G "MinGW Makefiles" -S ${SOURCE_DIR} -B ${BUILD_DIR} -DCMAKE_BUILD_TYPE=Debug
    RESULT_VARIABLE res
)
if(res)
    message(FATAL_ERROR "CMake configuration failed")
endif()

# 2. Build the project
message(STATUS "Building project...")
execute_process(
    COMMAND ${CMAKE_COMMAND} --build ${BUILD_DIR}
    RESULT_VARIABLE res
)
if(res)
    message(FATAL_ERROR "Build failed")
endif()

message(STATUS "Copying DLLs...")
execute_process(
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "lib/libraylib.dll" build/libraylib.dll
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "lib/libchipmunk.dll" build/libchipmunk.dll
    RESULT_VARIABLE res
)
if(res)
    message(FATAL_ERROR "DLLs copy error")
endif()