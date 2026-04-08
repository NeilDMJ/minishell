cmake_minimum_required(VERSION 4.2)
project(minishell C)

set(CMAKE_C_STANDARD 11)

include_directories(lib)

add_executable(minishell
        lib/minishell.c
        lib/minishell.h
        shell.c)
