if(BUILD_CONCEPTS_APP)
	add_subdirectory(concepts)
endif()

if(BUILD_DUMMY_CMAKE_APP)
	add_subdirectory(dummyCmake)
endif()

if(BUILD_ImageProcessSolution)
	add_subdirectory(ImageProcessArchitect)
endif()

if(BUILD_CV_APP)
	add_subdirectory(CVProject)
endif()