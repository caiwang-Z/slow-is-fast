set(TARGET myApp)

set(sources main.cpp)

# Find all .h files in the current directory and add them to the headers variable
file(GLOB headers "*.h")

add_executable(${TARGET} ${sources} ${headers})
