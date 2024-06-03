set(TARGET Core)

set(SOURCES Core.cpp ImageProcessingPipeline.cpp ImageStreamManager.cpp)

set(HEADERS Core.h ImageProcessingPipeline.h ImageStreamManager.h)

add_library(${TARGET} STATIC ${SOURCES} ${HEADERS})

set_property(TARGET Core PROPERTY POSITION_INDEPENDENT_CODE ON)

target_link_libraries(${TARGET} ImageStream ImageProcess Utils)

target_include_directories(${TARGET} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../ImageStream/" PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../Utils/" PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../ImageProcess/" PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../MAVision/")

add_dependencies(${TARGET} ImageStream ImageProcess Utils)

