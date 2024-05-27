set(TARGET image-process-demo-app)
add_executable(${TARGET} imageProcessDemoApp.cpp)

target_link_libraries(${TARGET} Core Utils)

target_include_directories(${TARGET} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../Core/" PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../ImageStream/" PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../Utils/" PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../ImageProcess/")

add_dependencies(${TARGET} Core Utils)


