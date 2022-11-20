cmake_minimum_required(VERSION 3.18)

project(
  fastest_sine
  VERSION "3.1.0"
  DESCRIPTION "Experiment into sine"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(src)
