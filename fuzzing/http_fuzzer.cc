// Copyright 2024
// Fuzzer for HTTP protocol

#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>

// Function for fuzzing HTTP protocol parser
int TestOneInput(const uint8_t* data, size_t size) {
  if (size < 4) {  // Minimum size for a valid HTTP request
    return 0;  // Not enough data for fuzzing
  }
  
  // Convert data to string form
  std::string input(reinterpret_cast<const char*>(data), size);
  
  // Here we would parse HTTP request
  // For a real implementation, we'd include the HTTP parser from the project
  // and check if it handles the input correctly without crashing
  
  // For example:
  // - Parse request line (GET /path HTTP/1.1)
  // - Parse headers
  // - Parse body if present
  
  // Record results of parsing for analysis
  int result_code = 0;
  std::string method;
  std::string path;
  std::string version;
  std::vector<std::pair<std::string, std::string>> headers;
  std::string body;
  
  // Very basic parsing just to demonstrate
  size_t pos = input.find(' ');
  if (pos != std::string::npos && pos < 10) {
    method = input.substr(0, pos);
    
    size_t path_start = pos + 1;
    size_t path_end = input.find(' ', path_start);
    
    if (path_end != std::string::npos) {
      path = input.substr(path_start, path_end - path_start);
      
      size_t version_start = path_end + 1;
      size_t version_end = input.find("\r\n", version_start);
      
      if (version_end != std::string::npos) {
        version = input.substr(version_start, version_end - version_start);
        result_code = 1;  // Successfully parsed request line
      }
    }
  }
  
  // To enable crash analysis, we can write the result to a file in a controlled manner
  if (getenv("HTTP_FUZZER_LOG")) {
    FILE* log_file = fopen(getenv("HTTP_FUZZER_LOG"), "a");
    if (log_file) {
      fprintf(log_file, "Result code: %d, Method: %s, Path: %s, Version: %s\n", 
              result_code, method.c_str(), path.c_str(), version.c_str());
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