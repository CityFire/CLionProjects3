cmake_minimum_required(VERSION 3.23)
project(CalculatorTest)

set(CMAKE_CXX_STANDARD 14)

add_executable(CalculatorTest main.cpp Tracer.cpp Tracer.h DebugNew.h)
