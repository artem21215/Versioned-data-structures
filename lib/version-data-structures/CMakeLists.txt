add_library(versioned-data-lib STATIC)

target_sources(versioned-data-lib
               PUBLIC
               include/VersionedTree.h src/VersionedTree.cpp
               )

target_include_directories(versioned-data-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
