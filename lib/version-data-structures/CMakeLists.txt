add_library(versioned-data-lib STATIC)

set_target_properties(versioned-data-lib PROPERTIES
            CXX_STANDARD 23
            CXX_EXTENSIONS OFF
            )

target_sources(versioned-data-lib
               PUBLIC
               include/HashSet.h
               include/StackHistory.h 
               include/StackState.h
               include/StackThreadController.h 
               include/VersionedStack.h
               include/VersionedSet.h
               include/VersionedTree.h
               include/VersionedTreeNode.h
               src/VersionedTree.cpp
               )

target_include_directories(versioned-data-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
