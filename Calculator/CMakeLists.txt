cmake_minimum_required(VERSION 3.23)
project(Calculator)

set(CMAKE_CXX_STANDARD 14)

add_executable(Calculator main.cpp Node.cpp Node.h Scanner.cpp Scanner.h Parser.cpp Parser.h SymbolTable.cpp SymbolTable.h)
