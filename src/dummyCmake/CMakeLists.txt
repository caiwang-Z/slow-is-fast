project(DummyCmakeDemo)

set(TARGET DummyCMakeDemo)

add_executable(${TARGET} dummyDemo.cpp thea.h)

target_link_libraries(${TARGET} PRIVATE DummyCalc_shared)
target_include_directories(${TARGET} PRIVATE ./dummyCalc)

# Ensure that DummyCalc_shared depends on calc target
add_dependencies(${TARGET} DummyCalc_shared)

#set_target_properties(${TARGET} PROPERTIES FOLDER "examples" VS_DEBUGGER_WORKING_DIRECTORY
#                                                                  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
#)

install(
  TARGETS ${TARGET}
  RUNTIME COMPONENT Runtime DESTINATION bin
)

add_subdirectory(dummyCalc)

