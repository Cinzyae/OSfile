cmake_minimum_required(VERSION 3.17)
project(OSfile C)

set(CMAKE_C_STANDARD 99)

add_executable(OSfile main.c disk.c disk.h filesys.c filesys.h cmd.c cmd.h ulib.c ulib.h cmd_sub.c cmd_sub.h)