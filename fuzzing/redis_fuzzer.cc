// Copyright 2024
// Fuzzer for Redis protocol

#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>

// Function for fuzzing Redis protocol parser
int TestOneInput(const uint8_t* data, size_t size) {
  if (size < 2) {
    return 0;  // Not enough data for fuzzing
  }
  
  // Convert data to string form
  std::string input(reinterpret_cast<const char*>(data), size);
  
  // Here we would parse Redis protocol commands
  // Redis protocol has several formats:
  // - Simple strings ("+OK\r\n")
  // - Errors ("-Error message\r\n")
  // - Integers (":1000\r\n")
  // - Bulk strings ("$5\r\nHello\r\n")
  // - Arrays ("*2\r\n$3\r\nGET\r\n$3\r\nkey\r\n")
  
  // For now, this is a placeholder for the actual Redis protocol parsing logic
  // In a real implementation, we would include the Redis protocol parser code
  // and check if it handles the input correctly

  // Record results of parsing for analysis
  int result_code = 0;
  std::string parsed_command;
  std::vector<std::string> parsed_args;
  
  // Try to identify the command type
  if (size > 0 && input[0] == '*') {
    // This is an array command
    result_code = 1;
  } else if (size > 0 && input[0] == '+') {
    // Simple string
    result_code = 2;
  } else if (size > 0 && input[0] == '-') {
    // Error
    result_code = 3;
  } else if (size > 0 && input[0] == ':') {
    // Integer
    result_code = 4;
  } else if (size > 0 && input[0] == '$') {
    // Bulk string
    result_code = 5;
  }
  
  // To enable crash analysis, we can write the result to a file in a controlled manner
  // This is just a placeholder for demonstration
  if (getenv("REDIS_FUZZER_LOG")) {
    FILE* log_file = fopen(getenv("REDIS_FUZZER_LOG"), "a");
    if (log_file) {
      fprintf(log_file, "Result code: %d, Input size: %zu\n", result_code, size);
      fclose(log_file);
    }
  }
  
  return 0;
}

// Standard entry point for AFL++
int main(int argc, char** argv) {
  // Check command line arguments
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
    return 1;
  }
  
  // Open input file
  FILE* file = fopen(argv[1], "rb");
  if (!file) {
    perror("Failed to open input file");
    return 1;
  }
  
  // Determine file size
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  fseek(file, 0, SEEK_SET);
  
  // Read file contents
  std::vector<uint8_t> buffer(file_size);
  size_t bytes_read = fread(buffer.data(), 1, file_size, file);
  fclose(file);
  
  if (bytes_read != static_cast<size_t>(file_size)) {
    fprintf(stderr, "Error reading file\n");
    return 1;
  }
  
  // Run fuzzing
  return TestOneInput(buffer.data(), buffer.size());
} 