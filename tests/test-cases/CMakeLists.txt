add_executable(test-cases VersionedTreeTest.cpp)

set_target_properties(test-cases PROPERTIES
            CXX_STANDARD 23
            CXX_EXTENSIONS OFF
            )

target_compile_options(test-cases PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

target_include_directories(test-cases PUBLIC ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

target_link_libraries(test-cases gtest gtest_main gmock_main versioned-data-lib)

add_test(test-cases ./test-cases)
