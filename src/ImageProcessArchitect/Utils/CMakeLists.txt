set(TARGET Utils)

set(SOURCES SIFLog.cpp debugdata.cpp)

set(HEADERS SIFLog.h debugdata.h)

add_library(${TARGET} STATIC ${SOURCES} ${HEADERS})

set_property(TARGET Core PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(${TARGET} PUBLIC .)