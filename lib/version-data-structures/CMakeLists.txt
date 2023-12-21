add_library(versioned-data-lib STATIC)

target_sources(versioned-data-lib
               PUBLIC
               include/HelloWorld.h src/HelloWorld.cpp
               )

target_include_directories(versioned-data-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
