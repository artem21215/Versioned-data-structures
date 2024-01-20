add_executable(versioned-data-app)

set_target_properties(versioned-data-app PROPERTIES
            CXX_STANDARD 23
            CXX_EXTENSIONS OFF
            )

target_sources(versioned-data-app
    PRIVATE
        "src/main.cpp"
)

target_include_directories(versioned-data-app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
link_directories(${CMAKE_BINARY_DIR}/lib/version-data-structures/Debug)
target_link_libraries(versioned-data-app PRIVATE versioned-data-lib)
