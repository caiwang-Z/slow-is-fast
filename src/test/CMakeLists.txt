cmake_minimum_required(VERSION 3.10)
enable_testing()
include_directories("${CMAKE_SOURCE_DIR}/3rdParty/googletest/googletest/include")

set(SOURCES main.cpp dummyTest.cpp)

set(HEADERS )

set(gtest_force_shared_crt off)

set(TARGET dummyTest)

add_executable(dummyTest ${SOURCES} ${HEADERS})

target_link_libraries(${TARGET} gtest gtest_main)

add_test(
	NAME dummyTest
	COMMAND $<TARGET_FILE:dummyTest>
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/$<CONFIG>/
)


#add_custom_command(
#			TARGET ${TARGET} POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E copy_directory
#				${CMAKE_CURRENT_SOURCE_DIR}/testData ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/Test
#			COMMENT "Copying TestData"
#)
