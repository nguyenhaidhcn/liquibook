
##-----------------------MACOS------------------
#cmake_minimum_required(VERSION 3.9)
#project(liquibook)
#include_directories(src/
#        src/book
#        src/mt_order_entry
#        /usr/local/include/activemq-cpp-3.9.4
#        /usr/local/include/activemq-cpp-3.10.0
##        /usr/include/apr-1.0
#        /usr/include/apr-1
##        /usr/include/apr-1
##        /usr/include/openssl
#        )
#
#file(GLOB SRCS
#        src/book/*.h
#        src/book/*.cpp
#        src/mt_order_entry/*.h
#        src/mt_order_entry/*.cpp)
#set(CMAKE_CXX_STANDARD 11)
#
#add_executable(liquibook src/mt_order_entry/mt_order_entry_main.cpp ${SRCS}
#        src/config/ConfigFile.cpp
#        src/config/ConfigFile.h
#        src/global/global.cpp
#        src/global/global.h
#        src/cms/CmsConsumer.h
#
#        src/cms/CmsProducer.h src/cms/CmsConsumer.cpp src/book/MatchingEngineAPI.h)
#
#
#
#add_library(activemq SHARED IMPORTED)
#set_target_properties(activemq PROPERTIES IMPORTED_LOCATION "/usr/local/lib/libactivemq-cpp.a")
#target_link_libraries(liquibook -lglog -pthread apr-1 ssl activemq
#        /usr/local/opt/openssl/lib/libssl.a
#        /usr/local/opt/openssl/lib/libcrypto.a
##        /usr/lib/x86_64-linux-gnu/libapr-1.a
##        /usr/lib/x86_64-linux-gnu/libssl.a
##        /usr/lib/x86_64-linux-gnu/libssl3.so
##        /usr/lib/apr-util-1/apr_crypto_openssl.a
#        )
##target_link_libraries(liquibook -lglog -pthread -lboost_thread -lboost_system -lboost_date_time -lrt -lboost_iostreams)
##-------------------MACOS-----------------------------------

#---------------------------CENT7---------------------------------
cmake_minimum_required(VERSION 3.9)
project(liquibook)
include_directories(src/
        src/book
        src/mt_order_entry
        /usr/include/apr-1
        /usr/include/activemq-cpp-3.9.3
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
        src/global/global.cpp
        src/global/global.h
        src/cms/CmsConsumer.h

        src/cms/CmsProducer.h src/cms/CmsConsumer.cpp src/book/MatchingEngineAPI.h)



add_library(activemq SHARED IMPORTED)
set_target_properties(activemq PROPERTIES IMPORTED_LOCATION "/usr/local/lib/libactivemq-cpp.a")
target_link_libraries(liquibook -pthread apr-1 ssl -ldl -luuid -lpthread -ldl -lssl -lcrypto activemq
        /usr/local/lib/libglog.a)
