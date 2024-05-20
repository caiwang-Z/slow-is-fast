set(TARGET dummyDemo)

add_executable(${TARGET} dummyDemo.cpp)

# target_link_libraries(${TARGET} PRIVATE opencv-world)

# add_dependencies(${TARGET} calc)

set_target_properties(${TARGET} PROPERTIES FOLDER "examples" VS_DEBUGGER_WORKING_DIRECTORY
                                                                  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

install(
  TARGETS ${TARGET}
  RUNTIME COMPONENT Runtime DESTINATION bin_tools
  )


