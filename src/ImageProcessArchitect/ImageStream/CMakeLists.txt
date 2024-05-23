set(TARGET ImageStream)

set(SOURCES DummyImageStream.cpp FrameBuffer.cpp IImageStream.cpp)

set(HEADERS DummyImageStream.h FrameBuffer.h IImageStream.h)

add_library(${TARGET} STATIC ${SOURCES} ${HEADERS})

set_property(TARGET Core PROPERTY POSITION_INDEPENDENT_CODE ON)

target_link_libraries(${TARGET} PRIVATE Utils)

target_include_directories(${TARGET} 
    PRIVATE $<TARGET_PROPERTY:Utils,INTERFACE_INCLUDE_DIRECTORIES>
)

add_dependencies(${TARGET} Utils)



