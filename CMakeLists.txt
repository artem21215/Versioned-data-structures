cmake_minimum_required(VERSION 3.27)
project(Versioned-Data-Structures VERSION 1.0 LANGUAGES CXX)

add_subdirectory(app)
add_subdirectory(lib)

enable_testing()
add_subdirectory(tests)
