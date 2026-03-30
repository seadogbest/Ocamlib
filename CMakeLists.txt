#begin of CMakeLists.txt
cmake_minimum_required( VERSION 2.8.10 )
cmake_policy( SET CMP0004 OLD )

#setting module name, must use the same value if parent module exists
set ( MODULE_APP_LIB_NAME Ocamavm )

#setting module version variable
set( CMAKE_INSTALL_RPATH_USE_LINK_PATH on )
set( CMAKE_VERBOSE_MAKEFILE off )

set( MAKEFLAGS $ENV{MAKEFLAGS})

set( prefix               "${CMAKE_INSTALL_PREFIX}"       )
set( EXEC_PREFIX          "${prefix}"                     )
set( LIBDIR               "${prefix}/lib/"                )
set( INCLUDEDIR           "${prefix}/include/"            )

#checking libraries dependency
include(FindPkgConfig)

set(_qnx_root "$ENV{QNX_TARGET}")
set(_qnx_lib_paths "")
set(_qnx_inc_paths "")
if(_qnx_root)
    list(APPEND _qnx_lib_paths
        "${_qnx_root}/aarch64le/usr/lib" "${_qnx_root}/aarch64le/lib"
        "${_qnx_root}/armle-v7/usr/lib" "${_qnx_root}/armle-v7/lib"
        "${_qnx_root}/x86_64/usr/lib" "${_qnx_root}/x86_64/lib"
    )
    list(APPEND _qnx_inc_paths
        "${_qnx_root}/aarch64le/usr/include"
        "${_qnx_root}/armle-v7/usr/include"
        "${_qnx_root}/x86_64/usr/include"
    )
endif()

pkg_check_modules(EGL egl)
pkg_check_modules(GLESV2 glesv2)

if(NOT EGL_FOUND)
    if(_qnx_root)
        find_library(EGL_LIBRARIES NAMES EGL PATHS ${_qnx_lib_paths} NO_DEFAULT_PATH)
    else()
        find_library(EGL_LIBRARIES NAMES EGL)
    endif()
endif()

if(NOT GLESV2_FOUND)
    if(_qnx_root)
        find_library(GLESV2_LIBRARIES NAMES GLESv2 GLESV2 PATHS ${_qnx_lib_paths} NO_DEFAULT_PATH)
    else()
        find_library(GLESV2_LIBRARIES NAMES GLESv2 GLESV2)
    endif()
endif()

if(NOT EGL_INCLUDE_DIRS AND _qnx_root)
    set(EGL_INCLUDE_DIRS ${_qnx_inc_paths})
endif()

if(NOT GLESV2_INCLUDE_DIRS AND _qnx_root)
    set(GLESV2_INCLUDE_DIRS ${_qnx_inc_paths})
endif()

if(NOT EGL_LIBRARIES)
    message(FATAL_ERROR "EGL library not found")
endif()

if(NOT GLESV2_LIBRARIES)
    message(FATAL_ERROR "GLESv2 library not found")
endif()

add_library(${MODULE_APP_LIB_NAME} 
    SHARED
    Ocamavm.cpp
    Ocamconfig.cpp
    mvtpublic.cpp
)

target_include_directories(${MODULE_APP_LIB_NAME}  PRIVATE
    ${EGL_INCLUDE_DIRS}
    ${GLESV2_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${MODULE_APP_LIB_NAME} 
    ${EGL_LIBRARIES}
    ${GLESV2_LIBRARIES}
)


target_compile_options(${MODULE_APP_LIB_NAME} 
    PRIVATE
        -DGL_GLEXT_PROTOTYPES
        -DEGL_EGLEXT_PROTOTYPES
        -Wno-unused-parameter
        -Wno-unused-variable
        -Wno-unused-function
        -Wno-macro-redefined
        -Wno-missing-field-initializers
        -Wno-writable-strings
        -Wno-uninitialized
        -Wno-format
        -Wno-unused-private-field
)

set(OCAMAVM_SOVERSION "0")
set(OCAMAVM_VERSION "${OCAMAVM_SOVERSION}.1")

set_target_properties(${MODULE_APP_LIB_NAME}  PROPERTIES
    VERSION ${OCAMAVM_VERSION}
    SOVERSION ${OCAMAVM_SOVERSION}
)

# ========= install dynamic library =========
install(TARGETS ${MODULE_APP_LIB_NAME}
    LIBRARY DESTINATION ${LIBDIR}
)

# ========= install headers =========
install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Ocamavm.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Ocamconfig.h
    DESTINATION ${INCLUDEDIR}
)

# ========= build demo executable =========
add_executable(demo demo/main.cpp)
target_include_directories(demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(demo
    ${MODULE_APP_LIB_NAME}
)

install(TARGETS demo
    DESTINATION bin
)

# =========生成demo可执行文件 ===============
