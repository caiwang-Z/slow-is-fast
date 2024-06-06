set(TARGET MAVision)

set(SOURCES mavision.cpp)

set(HEADERS mavision.h commondefinitions.h)

add_library(${TARGET} SHARED ${SOURCES} ${HEADERS})

target_compile_definitions(${TARGET} PRIVATE -DMAVISION_BUILD_DLL)


target_link_libraries(${TARGET} PRIVATE Core Utils ImageProcess)
add_dependencies(${TARGET} Core Utils ImageProcess)
target_include_directories(${TARGET} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../Core/")