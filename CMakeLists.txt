cmake_minimum_required(VERSION 3.10)
project(BatchRename)

set(CMAKE_CXX_STANDARD 17)
find_package(Boost COMPONENTS filesystem program_options REQUIRED)

add_executable(BatchRename main.cpp RefactorFilename.cpp RefactorFilename.h)
target_link_libraries(BatchRename ${Boost_LIBRARIES})