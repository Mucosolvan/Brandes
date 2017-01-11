cmake_minimum_required (VERSION 3.1)
project (MIMUW-CPCPP-lab1 C CXX)

find_package ( Threads REQUIRED )

#SET(CMAKE_C_COMPILER /usr/bin/gcc-4.9)
#SET(CMAKE_CXX_COMPILER /usr/bin/g++-4.9)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-g -O3 -Wall")

# http://stackoverflow.com/questions/10555706/
macro (add_executable _name)
    # invoke built-in add_executable
    _add_executable(${ARGV})
    if (TARGET ${_name})
        target_link_libraries(${_name} ${CMAKE_THREAD_LIBS_INIT} )
    endif()
endmacro()

add_executable(brandes-par brandes-par.cpp)
add_executable(brandes-seq brandes-seq.cpp)
add_executable(brandes-par2 brandes-par2.cpp)

install(TARGETS DESTINATION .)
