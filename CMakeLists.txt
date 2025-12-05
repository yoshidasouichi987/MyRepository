CMAKE_MINIMUM_REQUIRED(VERSION 3.14)

project(myProject CXX)
#コンパイル時に必要なオプション
add_definitions(-DDX_GCC_COMPILE)

set(PROJECT_VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)

#CMAKE_CURRENT_SOURCE_DIRは与えられている
set(PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
#set(PROJECT_PATH /home/yoshidasouichi987/MyCreator/myCmake)
#.cppファイルをSRC_FILESと定義
file(GLOB SRC_FILES "${PROJECT_PATH}/src/*.cpp")

add_executable(main ${SRC_FILES})

target_include_directories(main PUBLIC
    /usr/86_64-w64-mingw32/include
	${PROJECT_PATH}/include
	${PROJECT_PATH}/DxLib/include
)
target_link_directories(main PUBLIC
    /usr/x86_64-w64-mingw32/lib
    /usr/lib/gcc/x86_64-w64-mingw32/12
 	${PROJECT_PATH}/DxLib/lib
)

target_link_libraries(main PUBLIC 
    DxLib
    DxUseCLib
    DxDrawFunc
    jpeg
    png
    zlib
    tiff
    theora_static
    vorbis_static
    vorbisfile_static
    ogg_static
    bulletdynamics
    bulletcollision
    bulletmath
    opusfile
    opus
    silk_common
    celt
    #luser32
    #lgdi32
    #lwinmm
    #ldxguid
)


MESSAGE(STATUS "--------------------------------------------")
MESSAGE(STATUS "Info :")
#debug
message(STATUS "PROJECT_PATH = ${PROJECT_PATH}")
message(STATUS "SRC_FILES = ${SRC_FILES}")
MESSAGE(STATUS "  Project Name = ${PROJECT_NAME}")
MESSAGE(STATUS "  Version =     ${PROJECT_VERSION}")
MESSAGE(STATUS "  CMAKE_VERSION =        ${CMAKE_VERSION}")
MESSAGE(STATUS "  CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
MESSAGE(STATUS "  CMAKE_BUILD_TYPE =     ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "  CMAKE_INSTALL_LIBDIR = ${CMAKE_INSTALL_LIBDIR}")
