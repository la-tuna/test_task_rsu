SET(SOURCES
    lamp.cpp
    server.cpp
    client.cpp
)

SET(HEADERS
    lamp.h
    server.h
    client.h
)

ADD_LIBRARY(lamp STATIC lamp.cpp)
ADD_LIBRARY(server STATIC server.cpp)
ADD_LIBRARY(client STATIC client.cpp)

TARGET_INCLUDE_DIRECTORIES(lamp PUBLIC ${SRC_DIR})
TARGET_INCLUDE_DIRECTORIES(server PUBLIC ${SRC_DIR})
TARGET_INCLUDE_DIRECTORIES(client PUBLIC ${SRC_DIR})
