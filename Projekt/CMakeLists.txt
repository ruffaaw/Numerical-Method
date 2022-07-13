cmake_minimum_required(VERSION 3.21)
project(Projekt)

set(CMAKE_CXX_STANDARD 14)
include_directories(source)

add_executable(Projekt
        source/main.cpp
        source/calerf.cpp
        source/calerf.h
        source/equation_resolver.hpp
        source/thomas_algorithm.hpp
        source/vector_helper.hpp
        source/decomposition_lu.hpp)
