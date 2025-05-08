// Copyright 2024
// Fuzzer for Parse64Safe and Parse64Fast functions from base/flit.h

#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>

#include "base/flit.h"

// Use the namespace
using namespace base::flit;

// Function for fuzzing
int TestOneInput(const uint8_t* data, size_t size) {
  if (size < 1) {
    return 0;  // Not enough data for fuzzing
  }
  
  // Limit input size to prevent overflows
  const size_t max_size = 16;
  size_t use_size = (size > max_size) ? max_size : size;
  
  // Create a copy of the data
  std::vector<uint8_t> buffer(data, data + use_size);
  
  // Test the functions
  uint64_t result1 = 0;
  unsigned parsed1 = Parse64Fast(buffer.data(), &result1);
  
  uint64_t result2 = 0;
  unsigned parsed2 = Parse64Safe(buffer.data(), buffer.data() + buffer.size(), &result2);
  
  // Ignore results - we're only interested if crashes occur
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