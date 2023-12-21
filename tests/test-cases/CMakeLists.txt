add_executable(test-cases HelloWorldTest.cpp)

target_include_directories(test-cases PUBLIC ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

target_link_libraries(test-cases gtest gtest_main versioned-data-lib)

add_test(test-cases test-cases)
