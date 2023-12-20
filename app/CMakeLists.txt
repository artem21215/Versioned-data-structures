add_executable(versioned-data-app)

target_sources(versioned-data-app
    PRIVATE
        "src/main.cpp"
)

target_include_directories(versioned-data-app PUBLIC "include")
