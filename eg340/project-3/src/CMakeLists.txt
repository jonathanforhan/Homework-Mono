file(GLOB_RECURSE CXX_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

target_sources("${CMAKE_PROJECT_NAME}" PRIVATE "${CXX_SOURCES}")

if (MSVC)
    # Windows
    set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else ()
    # Unix
    set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -fno-exceptions")
endif ()

target_include_directories("${CMAKE_PROJECT_NAME}" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
