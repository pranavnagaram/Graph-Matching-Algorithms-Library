cmake_minimum_required(VERSION 3.11)
project(graphmatching)

set(CMAKE_CXX_STANDARD 17)

# Print the value of CMAKE_CURRENT_SOURCE_DIR
message(STATUS "Current source directory: ${CMAKE_CURRENT_SOURCE_DIR}")

# include directory and the libraries to build
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Print all files in the included directories

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE INCLUDE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*)
message(STATUS "Included files:")
foreach(INCLUDE_FILE ${INCLUDE_FILES})
    message(STATUS " - ${INCLUDE_FILE}")
endforeach()


message(STATUS "Current source directory: ${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(lib)
message(STATUS "Added LIB")
# add_subdirectory(test)
# message(STATUS "Added TEST")

add_executable(graphmatching main.cc)
target_link_libraries(graphmatching graph)

install(TARGETS graphmatching RUNTIME DESTINATION bin)
