# Specify the minimum CMAKE version required
cmake_minimum_required(VERSION 2.8)

project(loadGTMfiles)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if (COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif (COMPILER_SUPPORTS_CXX0X)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else ()
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif ()

#set(OpenCV_DIR "C:/Development/opencv/build/install")
find_package(OpenCV)

include_directories(${OpenCV_INCLUDE_DIRS} include)

if (WIN32)
  add_definitions(-DWINDOWS)
  include_directories(include/dirent)
endif ()

set(SRC "src/argvparser.cpp" "src/loadGTMfiles.cpp")

add_library(loadGTMfilesLib ${SRC})

add_executable(loadGTMfiles src/main.cpp)
target_link_libraries(loadGTMfiles loadGTMfilesLib ${OpenCV_LIBS})
