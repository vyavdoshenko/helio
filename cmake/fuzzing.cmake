# Settings for compiling fuzzers with AFL++

# Check for AFL++ presence
find_program(AFL_FUZZ afl-fuzz)
if(NOT AFL_FUZZ)
  message(FATAL_ERROR "AFL++ not found. Please install AFL++.")
endif()

# Function to add a fuzzer
function(add_fuzzer name source_file)
  # Create fuzzer executable
  add_executable(${name} ${source_file})
  
  # Add instrumentation options for AFL++
  target_compile_options(${name} PRIVATE -fsanitize=address)
  
  # Add linker options
  target_link_options(${name} PRIVATE -fsanitize=address)
  
  # Link with required libraries
  if(${ARGC} GREATER 2)
    target_link_libraries(${name} ${ARGN})
  endif()
  
  # Add target for running the fuzzer
  set(FUZZER_CORPUS_DIR "${CMAKE_CURRENT_BINARY_DIR}/${name}_corpus")
  set(FUZZER_FINDINGS_DIR "${CMAKE_CURRENT_BINARY_DIR}/${name}_findings")
  
  add_custom_target("run_${name}"
    COMMAND ${CMAKE_COMMAND} -E make_directory ${FUZZER_CORPUS_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${FUZZER_FINDINGS_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/corpus ${FUZZER_CORPUS_DIR}
    COMMAND AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES=1 ${AFL_FUZZ} -i ${FUZZER_CORPUS_DIR} -o ${FUZZER_FINDINGS_DIR} -m none -- $<TARGET_FILE:${name}> @@
    DEPENDS ${name}
    COMMENT "Running fuzzing for ${name}"
  )
endfunction() 