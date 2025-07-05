cmake_minimum_required(VERSION 3.10.0)
project(able VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

function(add_header MODULE_NAME)
    set(MODULE_ROOT_DIR "${CMAKE_SOURCE_DIR}/3rdParties/${MODULE_NAME}")
    
    # 1. 检查模块目录是否存在
    if(NOT EXISTS ${MODULE_ROOT_DIR})
        message(FATAL_ERROR "Module directory not found: ${MODULE_ROOT_DIR}")
    endif()

    # 2. 添加头文件目录
    if(EXISTS "${MODULE_ROOT_DIR}/include")
        target_include_directories(able PRIVATE "${MODULE_ROOT_DIR}/include")
    endif()
endfunction()

function(add_dll MODULE_NAME)
    set(MODULE_ROOT_DIR "${CMAKE_SOURCE_DIR}/3rdParties/${MODULE_NAME}")

    add_header(${MODULE_NAME})

    # 3. 处理库文件
    set(MODULE_LIB_DIR "${MODULE_ROOT_DIR}/lib/${CMAKE_BUILD_TYPE}")
    if(NOT EXISTS ${MODULE_LIB_DIR})
        message(FATAL_ERROR "Dynamic library directory not found: ${MODULE_LIB_DIR}")
    endif()

    target_link_directories(able PRIVATE ${MODULE_LIB_DIR})
    
    # 自动查找库文件
    if(WIN32)
        file(GLOB_RECURSE LIBS 
            "${MODULE_LIB_DIR}/*${MODULE_NAME}*.lib"
            "${MODULE_LIB_DIR}/*${MODULE_NAME}*.dll.lib"
        )
    else()
        file(GLOB LIBS "${MODULE_LIB_DIR}/*.so" "${MODULE_LIB_DIR}/*.dylib")
    endif()

    # 链接所有找到的库
    foreach(lib ${LIBS})
        target_link_libraries(able PRIVATE ${lib})
    endforeach()

    # 4. Windows平台特殊处理（复制运行时文件）
    if(WIN32)
        set(MODULE_BIN_DIR "${MODULE_ROOT_DIR}/bin/${CMAKE_BUILD_TYPE}")
        if(NOT EXISTS ${MODULE_BIN_DIR})
            message(FATAL_ERROR "Binary directory not found: ${MODULE_BIN_DIR}")
        endif()

        # 处理传入的文件类型参数
        set(file_types ${ARGN})
        if(NOT file_types)
            set(file_types "*.dll" "*.pdb")  # 默认复制dll和pdb
        endif()

        foreach(type ${file_types})
            file(GLOB files "${MODULE_BIN_DIR}/${type}")
            foreach(file ${files})
                add_custom_command(TARGET able POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        "${file}"
                        "$<TARGET_FILE_DIR:able>"
                    COMMENT "Copying ${file}"
                )
            endforeach()
        endforeach()
    endif()
endfunction()

function(add_sll MODULE_NAME)
    set(MODULE_ROOT_DIR "${CMAKE_SOURCE_DIR}/3rdParties/${MODULE_NAME}")

    add_header(${MODULE_NAME})

    # 处理静态库文件
    set(MODULE_LIB_DIR "${MODULE_ROOT_DIR}/lib/${CMAKE_BUILD_TYPE}")
    if(NOT EXISTS ${MODULE_LIB_DIR})
        message(FATAL_ERROR "Static library directory not found: ${MODULE_LIB_DIR}")
    endif()

    target_link_directories(able PRIVATE ${MODULE_LIB_DIR})
    
    # 自动查找静态库文件
    if(WIN32)
        file(GLOB_RECURSE LIBS 
            "${MODULE_LIB_DIR}/*${MODULE_NAME}*.lib"
            "${MODULE_LIB_DIR}/*${MODULE_NAME}*.static.lib"
            "${MODULE_LIB_DIR}/*${MODULE_NAME}*.dll.lib"
        )
    else()
        file(GLOB LIBS 
            "${MODULE_LIB_DIR}/*.a"
            "${MODULE_LIB_DIR}/*${MODULE_NAME}*.a"
        )
    endif()

    # 链接所有找到的静态库
    foreach(lib ${LIBS})
        target_link_libraries(able PRIVATE ${lib})
    endforeach()
endfunction()

file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/aspectable/*.cpp"
    "${CMAKE_SOURCE_DIR}/aspectable/*.h"
    "${CMAKE_SOURCE_DIR}/journal/*.cpp"
    "${CMAKE_SOURCE_DIR}/journal/*.h"
    "${CMAKE_SOURCE_DIR}/jsEngine/*.cpp"
    "${CMAKE_SOURCE_DIR}/jsEngine/*.h"
    "${CMAKE_SOURCE_DIR}/needle/*.cpp"
    "${CMAKE_SOURCE_DIR}/needle/*.h"
    "${CMAKE_SOURCE_DIR}/playable/*.cpp"
    "${CMAKE_SOURCE_DIR}/playable/*.h"
    "${CMAKE_SOURCE_DIR}/morph/*.cpp"
    "${CMAKE_SOURCE_DIR}/morph/*.h"
    "${CMAKE_SOURCE_DIR}/layout/*.cpp"
    "${CMAKE_SOURCE_DIR}/layout/*.h"
)
file(GLOB_RECURSE EXTRA_FILES
    "${CMAKE_SOURCE_DIR}/journal/*.json"
    "${CMAKE_SOURCE_DIR}/add-ins/*.js"
)

add_executable(able main.cpp ${SOURCES})
add_header(nlohmann)
add_dll(v8 "*.dll" "*.pdb" "*.dat" "*.bin")
add_dll(sdl "*.dll" "*.pdb")
add_sll(yoga)

foreach(extra_file ${EXTRA_FILES})
    add_custom_command(TARGET able POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${extra_file}"
            "$<TARGET_FILE_DIR:able>"
        COMMENT "Copying ${extra_file}"
    )
endforeach()
