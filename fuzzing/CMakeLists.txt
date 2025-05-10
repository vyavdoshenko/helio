# Fuzzing for Helio project
message(STATUS "Configuring fuzzers for Helio project")

# Check for AFL++ tools
find_program(AFL_CC afl-cc)
find_program(AFL_CXX afl-c++)
find_program(AFL_FUZZ afl-fuzz)

if(NOT AFL_CC OR NOT AFL_CXX OR NOT AFL_FUZZ)
  message(FATAL_ERROR "AFL++ tools not found. Please install AFL++.")
endif()

message(STATUS "Using compilers: ${AFL_CC} and ${AFL_CXX}")
message(STATUS "Fuzzer: ${AFL_FUZZ}")

# Ensure we use AFL++ compilers
set(CMAKE_C_COMPILER ${AFL_CC})
set(CMAKE_CXX_COMPILER ${AFL_CXX})

# Add fuzzer for functions from base/flit.h
add_fuzzer(flit_fuzzer flit_fuzzer.cc base)

# Add fuzzer for Redis protocol
add_fuzzer(redis_fuzzer redis_fuzzer.cc base)

# Add fuzzer for HTTP protocol
add_fuzzer(http_fuzzer http_fuzzer.cc base http_utils)

# Add analysis tool
add_executable(fuzzing_analyzer analysis.cc)
target_compile_features(fuzzing_analyzer PRIVATE cxx_std_17)
target_link_libraries(fuzzing_analyzer stdc++fs)

# Create a target for analyzing results for both protocols
add_custom_target(analyze_results
  COMMAND ${CMAKE_COMMAND} -E echo "Analyzing Redis fuzzing results..."
  COMMAND ${CMAKE_COMMAND} -E cat ${CMAKE_CURRENT_BINARY_DIR}/redis_fuzzer_findings/crashes/*
  COMMAND ${CMAKE_COMMAND} -E echo "Analyzing HTTP fuzzing results..."
  COMMAND ${CMAKE_COMMAND} -E cat ${CMAKE_CURRENT_BINARY_DIR}/http_fuzzer_findings/crashes/*
  DEPENDS run_redis_fuzzer run_http_fuzzer
  COMMENT "Analyzing fuzzing results"
)

# Create a target for analyzing Redis protocol results
add_custom_target(analyze_redis
  COMMAND ${CMAKE_COMMAND} -E echo "Analyzing Redis fuzzing results..."
  COMMAND ${CMAKE_COMMAND} -E cat ${CMAKE_CURRENT_BINARY_DIR}/redis_fuzzer_findings/crashes/*
  DEPENDS run_redis_fuzzer
  COMMENT "Analyzing Redis fuzzing results"
)

# Create a target for analyzing HTTP protocol results
add_custom_target(analyze_http
  COMMAND ${CMAKE_COMMAND} -E echo "Analyzing HTTP fuzzing results..."
  COMMAND ${CMAKE_COMMAND} -E cat ${CMAKE_CURRENT_BINARY_DIR}/http_fuzzer_findings/crashes/*
  DEPENDS run_http_fuzzer
  COMMENT "Analyzing HTTP fuzzing results"
)

# Create target for running full analysis with the tool
add_custom_target(detailed_analysis
  COMMAND $<TARGET_FILE:fuzzing_analyzer> 
          ${CMAKE_CURRENT_BINARY_DIR}/redis_fuzzer_findings 
          ${CMAKE_CURRENT_BINARY_DIR}/http_fuzzer_findings
  DEPENDS fuzzing_analyzer run_redis_fuzzer run_http_fuzzer
  COMMENT "Running detailed analysis of fuzzing results"
)

# Other potential fuzzers can be added here, for example:
# add_fuzzer(http_parser_fuzzer http_parser_fuzzer.cc base http_parser_lib)
# add_fuzzer(json_fuzzer json_fuzzer.cc base json_lib) 