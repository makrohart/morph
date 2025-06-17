cmake_minimum_required(VERSION 3.10.0)
project(able VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(able main.cpp)

set(V8_ROOT_DIR "${CMAKE_SOURCE_DIR}/3rdParties/v8")

# include directories
target_include_directories(able PRIVATE 
    "${V8_ROOT_DIR}/include"
)

# lib directories
target_link_directories(able PRIVATE 
    "${V8_ROOT_DIR}/lib/${CMAKE_BUILD_TYPE}"
)

# libs
target_link_libraries(able PRIVATE
    v8.dll.lib
    v8_libbase.dll.lib
    v8_libplatform.dll.lib
)
if (WIN32)
    file(GLOB V8_DLLS 
        "${V8_ROOT_DIR}/build/${CMAKE_BUILD_TYPE}/*.dll"
        "${V8_ROOT_DIR}/build/${CMAKE_BUILD_TYPE}/*.dat"
        "${V8_ROOT_DIR}/build/${CMAKE_BUILD_TYPE}/*.bin"
    )
    foreach(dll ${V8_DLLS})
        add_custom_command(TARGET able POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${dll} $<TARGET_FILE_DIR:able>
            COMMENT "Copying V8 DLLs to output directory"
        )
    endforeach()
endif()

