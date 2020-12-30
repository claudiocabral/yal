cmake_minimum_required(VERSION 3.13.4)
project(sl)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(extern/Catch2)

find_package(LLVM REQUIRED CONFIG)
find_package(Threads REQUIRED)

set(NAME sl)
set(LIBRARY lib${NAME})
set(EXECUTABLE ${NAME})
set(TEST test-${NAME})

set(COMPILE_OPTIONS
    -Werror
    -Wall
    -Wextra
    -Wno-unused-parameter
    -Wno-unused-variable
    )

set(INCLUDES
    includes
    extern/frozen/include
    )

  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")

add_executable(${EXECUTABLE} srcs/main.cpp)
add_executable(${TEST}
  tests/main.cpp
  tests/lexer/simple_lexer.cpp
  )
add_library(${LIBRARY}
    srcs/lexer.cpp
    srcs/parser.cpp
    )
set_target_properties(${LIBRARY} PROPERTIES OUTPUT_NAME ${NAME})

set_target_properties(${LIBRARY} ${EXECUTABLE} ${TEST}
    PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    )

target_link_libraries(${LIBRARY} LLVM Threads::Threads)
target_include_directories(${LIBRARY} PUBLIC ${INCLUDES} ${LLVM_INCLUDE_DIRS})
target_compile_definitions(${LIBRARY} PUBLIC ${LLVM_DEFINITIONS})
target_compile_options(${LIBRARY} PUBLIC ${COMPILE_OPTIONS})

target_link_libraries(${EXECUTABLE} ${LIBRARY})
target_link_libraries(${TEST} ${LIBRARY} Catch2::Catch2)
