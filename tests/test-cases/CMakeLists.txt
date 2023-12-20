add_executable(test-cases PointTests.cpp PolyLineTests.cpp VectorTests.cpp SegmentTests.cpp UtilitesTests.cpp EuclideanCoordsTests.cpp)

target_include_directories(test-cases PUBLIC ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

target_link_libraries(test-cases gtest gtest_main)
target_link_libraries(test-cases versioned-data-lib)
