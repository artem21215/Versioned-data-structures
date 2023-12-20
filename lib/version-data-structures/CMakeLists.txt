add_library(versioned-data-lib SHARED)

target_sources(versioned-data-lib
               PUBLIC
               include/HelloWorld.h src/HelloWorld.cpp
               )

target_include_directories(versioned-data-lib PUBLIC "include")
