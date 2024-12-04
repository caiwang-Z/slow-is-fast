project(DummyCalc)

set(TARGET DummyCalc)

set(SOURCES calc.cpp)
set(HEADERS calc.h)

add_library(${TARGET}_obj OBJECT ${SOURCES} ${HEADERS})
target_compile_definitions(${TARGET}_obj PRIVATE -DBUILD_CALC_DLL)

# Create a static library from the object files
add_library(${TARGET}_static STATIC $<TARGET_OBJECTS:${TARGET}_obj>)

# Create a shared library from the object files
add_library(${TARGET}_shared SHARED $<TARGET_OBJECTS:${TARGET}_obj>)

# Group static and shared libraries under 'app_libraries' in IDE
set_target_properties(${TARGET}_static ${TARGET}_shared PROPERTIES FOLDER "app_libraries")

# Install headers to include directory
install(FILES ${HEADERS} DESTINATION include)

# Install static library to lib directory
install(TARGETS ${TARGET}_static DESTINATION lib)

# Install shared library DLL to bin directory and import library (LIB) to lib directory
install(TARGETS ${TARGET}_shared
    RUNTIME DESTINATION bin  # For .dll files
    ARCHIVE DESTINATION lib  # For .lib files
    LIBRARY DESTINATION lib  # For Unix-based shared libraries
)