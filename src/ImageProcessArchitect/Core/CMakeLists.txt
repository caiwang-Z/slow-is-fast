set(TARGET Core)

set(SOURCES Core.cpp ImageProcessingPipeline.cpp ImageStreamManager.cpp)

set(HEADERS Core.h ImageProcessingPipeline.h ImageStreamManager.h)

add_library(${TARGET} STATIC ${SOURCES} ${HEADERS})

set_property(TARGET Core PROPERTY POSITION_INDEPENDENT_CODE ON)

target_link_libraries(${TARGET} ImageStream ImageProcess)

add_dependencies(${TARGET} ImageStream ImageProcess)

