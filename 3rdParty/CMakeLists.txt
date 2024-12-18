if(BUILD_CV_APP)
	add_subdirectory(opencv)
endif()

if(BUILD_GOOGLETEST)
	add_subdirectory(googletest)
endif()
