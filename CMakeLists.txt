#Make 最低版本
cmake_minimum_required(VERSION 2.1)

#set project name
project(rtsp_project C)

# set c++ version
#set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 99)


#set debug
set(CMAKE_BUILD_TYPE Debug)

#set release + debug infor
#set(CMAKE_BUILD_TYPE RelWithDebInfo)
#add source file, cpp file add to parameter SOURCES
file(GLOB_RECURSE SOURCES "src/*.c")

# 添加库的头文件

#debug 
message(STATUS "Sources: ${SOURCES}")

# to static library .a(achive library)
# to share library .so(shared object)
#add_library(network_program SHARE ${SOURCES} ${HEADERS})

#cpp to exe file
#add_executable(rtsp_program ${SOURCES})
#install .a or .so to lib, .exe to bin

#install header file to include

#add h files
#PRIVATE表示这个头文件只对network_program有效，其它使用network_program的程序无效，
#public即引用这个库的程序也有效

# 链接库（假设有个 mylib 库）
#target_link_libraries(rtsp_program PRIVATE libavformat.so、libavcodec、libavutil、libswscale) 


# 查找 PkgConfig 和 FFmpeg 库
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG REQUIRED libavformat libavcodec libavutil libswscale)

# 打印 FFmpeg 的头文件和库路径（调试用）
message(STATUS "FFmpeg include directories: ${FFMPEG_INCLUDE_DIRS}")
message(STATUS "FFmpeg libraries: ${FFMPEG_LIBRARIES}")

# 添加 FFmpeg 的头文件路径
include_directories(${FFMPEG_INCLUDE_DIRS})

#set(CMAKE_EXE_LINKER_FLAGS "-static")

# 生成可执行文件
add_executable(rtsp_program ${SOURCES})

# 链接 FFmpeg 库
target_link_libraries(rtsp_program PRIVATE ${FFMPEG_LIBRARIES})
