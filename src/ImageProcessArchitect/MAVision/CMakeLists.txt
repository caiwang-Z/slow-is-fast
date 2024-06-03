set(TARGET MAVision)

set(SOURCES mavision.cpp)

set(HEADERS mavision.h commondefinitions.h)

add_library(${TARGET} SHARED ${SOURCES} ${HEADERS})

target_compile_definitions(${TARGET} PRIVATE -DMAVISION_BUILD_DLL)


target_link_libraries(${TARGET} PRIVATE Core Utils)
add_dependencies(${TARGET} Core Utils)
target_include_directories(${TARGET} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../Core/")