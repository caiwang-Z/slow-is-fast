set(TARGET myCVApp)

set(sources main.cpp)

# Find all .h files in the current directory and add them to the headers variable
file(GLOB headers "*.h")

add_executable(${TARGET} ${sources} ${headers})

target_link_libraries(${TARGET} PRIVATE opencv_core opencv_imgproc opencv_imgcodecs)

target_include_directories(${TARGET} PRIVATE "${CMAKE_BINARY_DIR}/" "${CMAKE_SOURCE_DIR}/3rdParty/opencv/include/")

set(MODULES_DIR "${CMAKE_SOURCE_DIR}/3rdParty/opencv/modules/")

# TODO stupid, check the flags and how to use it gentally
target_include_directories(${TARGET} PRIVATE "${MODULES_DIR}/highgui/include" "${MODULES_DIR}/imgcodecs/include" "${MODULES_DIR}/imgproc/include" "${MODULES_DIR}/core/include" "${MODULES_DIR}/calib3d/include" "${MODULES_DIR}/features2d/include"
"${MODULES_DIR}/flann/include"
"${MODULES_DIR}/dnn/include"
"${MODULES_DIR}/videoio/include"
"${MODULES_DIR}/ml/include"
"${MODULES_DIR}/objdetect/include"
"${MODULES_DIR}/photo/include"
"${MODULES_DIR}/stitching/include"
"${MODULES_DIR}/video/include"
)