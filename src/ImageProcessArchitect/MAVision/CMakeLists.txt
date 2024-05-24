set(TARGET MAVision)

set(SOURCES mavision.cpp)

set(HEADERS mavision.h)

add_library(${TARGET} STATIC ${SOURCES} ${HEADERS})

target_link_libraries(${TARGET} PRIVATE Core Utils)
add_dependencies(${TARGET} Core Utils)
target_include_directories(${TARGET} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../Core/")