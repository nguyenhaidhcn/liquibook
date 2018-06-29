cmake_minimum_required(VERSION 3.9)
project(liquibook)
include_directories(src/
        src/book
        src/mt_order_entry
        /usr/local/include/activemq-cpp-3.9.4
        /usr/local/include/activemq-cpp-3.10.0
        /usr/include/apr-1.0
        /usr/include/apr-1
        )

file(GLOB SRCS
        src/book/*.h
        src/book/*.cpp
        src/mt_order_entry/*.h
        src/mt_order_entry/*.cpp)
set(CMAKE_CXX_STANDARD 11)

add_executable(liquibook src/mt_order_entry/mt_order_entry_main.cpp ${SRCS}
        src/config/ConfigFile.cpp
        src/config/ConfigFile.h
        global/global.cpp
        global/global.h
        src/cms/AsyncGwConsumer.h

        )



add_library(activemq SHARED IMPORTED)
set_target_properties(activemq PROPERTIES IMPORTED_LOCATION "/usr/local/lib/libactivemq-cpp.a")
target_link_libraries(liquibook -lglog -pthread activemq)
#target_link_libraries(liquibook -lglog -pthread -lboost_thread -lboost_system -lboost_date_time -lrt -lboost_iostreams)
