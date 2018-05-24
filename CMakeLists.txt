project(NGAS)
cmake_minimum_required(VERSION 3.5.1)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
if(${CMAKE_VERSION} VERSION_LESS "3.8.0") 
  message(WARNING "Please switch to CMake above 3.8.0 which enables CXX_STANDARD 17")
  set(CMAKE_CXX_STANDARD 14)
endif()

# Build type
if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS_DEBUG "-g -DHUSKY_DEBUG_MODE")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g -DHUSKY_DEBUG_MODE")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -w")
endif()

# Generate position independent code
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Dependencies
find_package(gflags REQUIRED)
message(STATUS "Found gflags:")
message(STATUS "  (Headers)       ${gflags_INCLUDE_DIR}")
message(STATUS "  (Library)       ${gflags_LIBRARIES}")
include_directories(${gflags_INCLUDE_DIR})

find_path(glog_INCLUDE_DIR NAMES glog/logging.h)
find_library(glog_LIBRARY NAMES glog)
message(STATUS "Found glog:")
message(STATUS "  (Headers)       ${glog_INCLUDE_DIR}")
message(STATUS "  (Library)       ${glog_LIBRARY}")
include_directories(${glog_INCLUDE_DIR})

find_path(LIBHDFS3_INCLUDE_DIR NAMES hdfs/hdfs.h)
find_library(LIBHDFS3_LIBRARY NAMES hdfs3)
message(STATUS "Found libhdfs3:")
message(STATUS "  (Headers)       ${LIBHDFS3_INCLUDE_DIR}")
message(STATUS "  (Library)       ${LIBHDFS3_LIBRARY}")
include_directories(${LIBHDFS3_INCLUDE_DIR})

# NGAS
include_directories(${PROJECT_SOURCE_DIR})

# Data files
file(GLOB data-src-files
  data/vertex.cc
)

# data-static
add_library(data-static STATIC ${data-src-files})
target_link_libraries(data-static gflags ${glog_LIBRARY} hdfs3)
set_property(TARGET data-static PROPERTY CXX_STANDARD ${CMAKE_CXX_STANDARD})
