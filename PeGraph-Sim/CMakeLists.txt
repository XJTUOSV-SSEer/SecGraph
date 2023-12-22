# 设置最低的CMake版本要求
cmake_minimum_required(VERSION 3.10)

# 项目名称
project(OXTProtocol)

# 指定C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加可执行文件
add_executable(OXTmain OXTmain.cpp)