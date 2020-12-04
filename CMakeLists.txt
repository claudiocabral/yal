cmake_minimum_required(VERSION 3.13.4)
project(sl)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(LLVM REQUIRED CONFIG)

#llvm_map_components_to_libnames(llvm_libs llvm)

set(EXECUTABLE sl)
set(COMPILE_OPTIONS
    -Werror
    -Wall
    -Wextra
    -Wno-unused-parameter
    -Wno-unused-variable
    -Wno-unused-but-set-variable
    )

add_executable(${EXECUTABLE} srcs/main.cpp)


set_target_properties(${EXECUTABLE}
    PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    )

target_link_libraries(${EXECUTABLE} ${llvm_libs} LLVM)
target_include_directories(${EXECUTABLE} PRIVATE ${LLVM_INCLUDE_DIRS})
target_compile_definitions(${EXECUTABLE} PUBLIC ${LLVM_DEFINITIONS})
target_compile_options(${EXECUTABLE} PUBLIC ${COMPILE_OPTIONS})
