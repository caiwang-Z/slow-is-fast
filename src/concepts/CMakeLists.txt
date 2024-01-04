set(sources main.cpp)
set(headers headerTest.h)

set(PROJECT_TARGET slow-is-fast)

add_executable(${PROJECT_TARGET} ${sources} ${headers})
