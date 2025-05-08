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

# Other potential fuzzers can be added here, for example:
# add_fuzzer(http_parser_fuzzer http_parser_fuzzer.cc base http_parser_lib)
# add_fuzzer(json_fuzzer json_fuzzer.cc base json_lib) 