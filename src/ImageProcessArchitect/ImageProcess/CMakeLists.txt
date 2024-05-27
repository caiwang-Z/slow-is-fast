set(TARGET ImageProcess)

set(SOURCES DummyImageOperation.cpp)

set(HEADERS DummyImageOperation.h)

add_library(${TARGET} STATIC ${SOURCES} ${HEADERS})

set_property(TARGET Core PROPERTY POSITION_INDEPENDENT_CODE ON)


