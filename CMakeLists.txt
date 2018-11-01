cmake_minimum_required(VERSION 3.9)
project(stl)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable")
add_executable(stltest test/test.cc alloc.cc mycstring.c mystring.cc)

# -Wextra
# -Wall  打开gcc的所有警告
# -Werror，它要求gcc将所有的警告当成错误进行处理

