cmake_minimum_required(VERSION 3.9)
project(liquibook)
include_directories(src/
        src/book)

file(GLOB SRCS
        src/book/*.h *.cpp)
set(CMAKE_CXX_STANDARD 14)

add_executable(liquibook src/book/main.cpp ${SRCS})
target_link_libraries(liquibook -lglog -pthread -lboost_thread -lboost_system -lboost_date_time -lrt -lboost_iostreams)
