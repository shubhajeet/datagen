cmake_minimum_required(VERSION 3.12)
project(datagen)

set(CMAKE_CXX_STANDARD 20)
find_package(gflags REQUIRED)
find_package(yaml-cpp REQUIRED)

# find_package(spdlog REQUIRED)

# include_directories(/home/sujit/lib/spdlog/include)
# include_directories(/home/sujit/data/spdlog/include/spdlog)
include_directories(${YAML_CPP_INCLUDE_DIRS})

# Define the macro ‘DEBUG' in the debug mode
if(CMAKE_BUILD_TYPE STREQUAL Debug)
    ADD_DEFINITIONS(-DDEBUG)
endif()

# Add -O0 to remove optimizations when using gcc
IF(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

if(MSVC)
    set(CMAKE_CXX_FLAGS "/O2 /arch:AVX2 /W1 /EHsc")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    set(CMAKE_CXX_FLAGS "-O3 -xHost")
else()
    # clang and gcc
    set(CMAKE_CXX_FLAGS "-O3 -march=native -Wall -Wextra")
endif()

include_directories(src/core)

# set(DOCTEST_DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/doctest)
# file(DOWNLOAD
# https://raw.githubusercontent.com/onqtam/doctest/2.4.6/doctest/doctest.h
# ${DOCTEST_DOWNLOAD_DIR}/doctest.h
# EXPECTED_HASH SHA1=40728d2bed7f074e80cb095844820114e7d16ce0
# )
add_executable(datagen src/main.cpp)
add_executable(generator src/generator.cpp)
add_executable(perflinear src/perflinear.cpp)
add_executable(gentrace src/gentrace.cpp)
add_executable(combinetrace src/combinetrace.cpp)
target_link_libraries(datagen ${YAML_CPP_LIBRARIES} ${gflags_LIBRARIES})
target_link_libraries(generator ${YAML_CPP_LIBRARIES} ${gflags_LIBRARIES})
target_link_libraries(perflinear ${YAML_CPP_LIBRARIES} ${gflags_LIBRARIES})
target_link_libraries(gentrace ${YAML_CPP_LIBRARIES} ${gflags_LIBRARIES})
target_link_libraries(combinetrace ${YAML_CPP_LIBRARIES} ${gflags_LIBRARIES})

# target_include_directories(test_alex PRIVATE ${DOCTEST_DOWNLOAD_DIR})

# enable_testing()
# add_test(test_alex test_alex)