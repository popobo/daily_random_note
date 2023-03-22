cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_CLANG_TIDY
    clang-tidy;
    -format-style='file';
    -checks=cppcoreguidelines*
    -header-filter=${CMAKE_CURRENT_SOURCE_DIR};
)

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}" -fsanitize=address)
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG}" -fsanitize=address)

include_directories(${PROJECT_SOURCE_DIR})
add_executable(writie main.cpp FileWriter.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)