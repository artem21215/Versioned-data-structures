add_library(versioned-data-lib STATIC)

set_target_properties(versioned-data-lib PROPERTIES
            CXX_STANDARD 23
            CXX_EXTENSIONS OFF
            )

target_sources(versioned-data-lib
               PUBLIC
               include/VersionedTree.h src/VersionedTree.cpp include/VersionedTreeNode.h include/VersionedStack.h
               include/StackThreadController.h include/StackHistory.h include/StackState.h
               )

target_include_directories(versioned-data-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
