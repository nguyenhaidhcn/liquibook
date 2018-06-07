cmake_minimum_required(VERSION 3.9)
project(liquibook)
include_directories(src/
        src/book
        src/mt_order_entry)

file(GLOB SRCS
        src/book/*.h
        src/book/*.cpp
        src/mt_order_entry/*.h
        src/mt_order_entry/*.cpp)
set(CMAKE_CXX_STANDARD 11)

add_executable(liquibook src/mt_order_entry/mt_order_entry_main.cpp ${SRCS})
target_link_libraries(liquibook -lglog -pthread -lboost_thread -lboost_system -lboost_date_time -lrt -lboost_iostreams)
