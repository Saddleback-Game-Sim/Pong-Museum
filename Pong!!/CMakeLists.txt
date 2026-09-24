cmake_minimum_required(VERSION 3.20)
project(RaylibTemplate LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/utf-8)
endif()

if(WIN32)
    set(RAYLIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/raylib/include")

    if(MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 4)
        message(FATAL_ERROR
            "The bundled MSVC raylib libraries are x64, but the current generator platform is Win32. "
            "Please reconfigure CMake with a 64-bit generator, for example: cmake -S . -B build -A x64"
        )
    endif()

    if(NOT MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 4)
        message(FATAL_ERROR
            "The bundled MinGW raylib libraries are x64, but the current toolchain is 32-bit. "
            "Please reconfigure CMake with a 64-bit MinGW toolchain."
        )
    endif()

    if(NOT EXISTS "${RAYLIB_INCLUDE_DIR}/raylib.h")
        message(FATAL_ERROR "Bundled raylib headers were not found in: ${RAYLIB_INCLUDE_DIR}")
    endif()

    include_directories(SYSTEM ${RAYLIB_INCLUDE_DIR})

    if(MSVC)
        set(RAYLIB_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/raylib/lib/msvc")
        if(EXISTS "${RAYLIB_LIB_DIR}/raylibdll.lib")
            set(RAYLIB_IMPORT_LIB "${RAYLIB_LIB_DIR}/raylibdll.lib")
        else()
            set(RAYLIB_IMPORT_LIB "${RAYLIB_LIB_DIR}/raylib.lib")
        endif()
        set(RAYLIB_RUNTIME_FILE "${RAYLIB_LIB_DIR}/raylib.dll")
    else()
        set(RAYLIB_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/raylib/lib/mingw")
        set(RAYLIB_IMPORT_LIB "${RAYLIB_LIB_DIR}/libraylib.dll.a")
        set(RAYLIB_RUNTIME_FILE "${RAYLIB_LIB_DIR}/libraylib.dll")
    endif()

    if(NOT EXISTS "${RAYLIB_IMPORT_LIB}")
        message(FATAL_ERROR "Bundled raylib import library was not found: ${RAYLIB_IMPORT_LIB}")
    endif()

    if(NOT EXISTS "${RAYLIB_RUNTIME_FILE}")
        message(FATAL_ERROR "Bundled raylib runtime DLL was not found: ${RAYLIB_RUNTIME_FILE}")
    endif()

    add_library(raylib SHARED IMPORTED GLOBAL)
    set_target_properties(raylib PROPERTIES
        IMPORTED_IMPLIB "${RAYLIB_IMPORT_LIB}"
        IMPORTED_LOCATION "${RAYLIB_RUNTIME_FILE}"
        INTERFACE_INCLUDE_DIRECTORIES "${RAYLIB_INCLUDE_DIR}"
    )
elseif(APPLE)
    set(RAYLIB_SEARCH_ROOTS
        /opt/homebrew
        /usr/local
        /opt/local
    )

    find_path(
        RAYLIB_INCLUDE_DIR
        raylib.h
        HINTS ${RAYLIB_SEARCH_ROOTS}
        PATH_SUFFIXES include
        REQUIRED
    )

    find_library(
        RAYLIB_LIBRARY
        raylib
        HINTS ${RAYLIB_SEARCH_ROOTS}
        PATH_SUFFIXES lib
        REQUIRED
    )

    add_library(raylib UNKNOWN IMPORTED GLOBAL)
    set_target_properties(raylib PROPERTIES
        IMPORTED_LOCATION "${RAYLIB_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${RAYLIB_INCLUDE_DIR}"
    )
elseif(UNIX)
    find_package(PkgConfig QUIET)

    if(PkgConfig_FOUND)
        pkg_check_modules(RAYLIB_PC QUIET IMPORTED_TARGET raylib)
    endif()

    if(RAYLIB_PC_FOUND)
        add_library(raylib INTERFACE IMPORTED GLOBAL)
        target_link_libraries(raylib INTERFACE PkgConfig::RAYLIB_PC)
    else()
        find_path(RAYLIB_INCLUDE_DIR raylib.h)
        find_library(RAYLIB_LIBRARY NAMES raylib libraylib)

        if(NOT RAYLIB_INCLUDE_DIR OR NOT RAYLIB_LIBRARY)
            message(FATAL_ERROR
                "Linux raylib development files were not found. "
                "On Ubuntu/Debian install them with: sudo apt install libraylib-dev"
            )
        endif()

        add_library(raylib UNKNOWN IMPORTED GLOBAL)
        set_target_properties(raylib PROPERTIES
            IMPORTED_LOCATION "${RAYLIB_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${RAYLIB_INCLUDE_DIR}"
        )
    endif()
else()
    message(FATAL_ERROR "This template currently supports Windows, macOS, and Linux only.")
endif()

add_library(raylib_media STATIC thirdparty/raylib-media/rmedia.c)
set_target_properties(raylib_media PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED YES)
target_include_directories(raylib_media PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/raylib-media")
target_link_libraries(raylib_media PUBLIC raylib)

if(WIN32)
    set(FFMPEG_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/ffmpeg" CACHE PATH "FFmpeg development package")
    find_path(FFMPEG_INCLUDE_DIR libavcodec/avcodec.h PATHS "${FFMPEG_ROOT}/include" NO_DEFAULT_PATH REQUIRED)
    target_include_directories(raylib_media SYSTEM PRIVATE "${FFMPEG_INCLUDE_DIR}")
    foreach(component avformat avcodec avutil swresample swscale)
        find_library(FFMPEG_${component}_LIBRARY NAMES ${component}
            PATHS "${FFMPEG_ROOT}/lib" NO_DEFAULT_PATH REQUIRED)
        target_link_libraries(raylib_media PRIVATE "${FFMPEG_${component}_LIBRARY}")
    endforeach()
    file(GLOB FFMPEG_RUNTIME_FILES CONFIGURE_DEPENDS "${FFMPEG_ROOT}/bin/*.dll")
    if(NOT FFMPEG_RUNTIME_FILES)
        message(FATAL_ERROR "FFmpeg runtime DLLs were not found in ${FFMPEG_ROOT}/bin")
    endif()
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
        libavformat libavcodec libavutil libswresample libswscale)
    target_link_libraries(raylib_media PRIVATE PkgConfig::FFMPEG)
endif()

add_executable(RaylibTemplate main.cpp)

target_link_libraries(RaylibTemplate PRIVATE raylib_media)

# An always-run resource target also picks up video changes without relinking.
add_custom_target(pong_resources ALL
    COMMAND "${CMAKE_COMMAND}" -E make_directory "$<TARGET_FILE_DIR:RaylibTemplate>/res"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/res/test.mp4" "$<TARGET_FILE_DIR:RaylibTemplate>/res/test.mp4"
    VERBATIM
)
add_dependencies(RaylibTemplate pong_resources)

if(WIN32)
    if(MINGW)
        get_filename_component(mingw_bin "${CMAKE_CXX_COMPILER}" DIRECTORY)
        foreach(runtime libstdc++-6.dll libgcc_s_seh-1.dll libwinpthread-1.dll)
            execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "-print-file-name=${runtime}"
                OUTPUT_VARIABLE runtime_path OUTPUT_STRIP_TRAILING_WHITESPACE
                COMMAND_ERROR_IS_FATAL ANY)
            if(NOT EXISTS "${runtime_path}" AND EXISTS "${mingw_bin}/${runtime}")
                set(runtime_path "${mingw_bin}/${runtime}")
            endif()
            if(NOT EXISTS "${runtime_path}")
                message(FATAL_ERROR "MinGW runtime ${runtime} was not found by ${CMAKE_CXX_COMPILER}")
            endif()
            list(APPEND FFMPEG_RUNTIME_FILES "${runtime_path}")
        endforeach()
    endif()
    add_custom_command(TARGET RaylibTemplate POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "${RAYLIB_RUNTIME_FILE}"
            "$<TARGET_FILE_DIR:RaylibTemplate>"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            ${FFMPEG_RUNTIME_FILES}
            "$<TARGET_FILE_DIR:RaylibTemplate>"
        VERBATIM
    )
endif()
