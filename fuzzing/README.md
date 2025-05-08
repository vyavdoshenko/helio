# Fuzzing for Helio Project with AFL++

This directory contains tools for fuzzing Helio components using [AFL++](https://github.com/AFLplusplus/AFLplusplus).

## Dependencies

Before starting, you need to install AFL++:

```bash
# On Ubuntu/Debian
sudo apt-get install build-essential llvm-dev libclang-dev clang
git clone https://github.com/AFLplusplus/AFLplusplus
cd AFLplusplus
make
sudo make install
```

## Available Fuzzers

1. **flit_fuzzer** - fuzzer for testing Parse64Fast and Parse64Safe functions from base/flit.h.

## How to Build and Run

### Method 1: Using the Script

Simply run:

```bash
./build-and-run.sh
```

### Method 2: Manual Build and Run

```bash
# Create a build directory
mkdir -p build-fuzzing && cd build-fuzzing

# Configure the project
CC=afl-cc CXX=afl-c++ cmake .. -DENABLE_FUZZING=ON -DCMAKE_BUILD_TYPE=Debug

# Build the project
make -j$(nproc) flit_fuzzer

# Run the fuzzer
make run_flit_fuzzer
```

## Adding New Fuzzers

To add a new fuzzer:

1. Create a new .cc file named `your_fuzzer.cc`
2. Implement a standard main function that reads input from a file
3. Add the executable to fuzzing/CMakeLists.txt using the add_fuzzer function
4. Create an initial corpus in the corpus directory

Example:
```cpp
// my_fuzzer.cc
#include <cstdint>
#include <vector>

int TestOneInput(const uint8_t* data, size_t size) {
  // Fuzzing logic here
  return 0;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    return 1;
  }
  
  // Open and read input file
  FILE* file = fopen(argv[1], "rb");
  if (!file) return 1;
  
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);
  
  std::vector<uint8_t> buffer(size);
  fread(buffer.data(), 1, size, file);
  fclose(file);
  
  return TestOneInput(buffer.data(), buffer.size());
}
```

Then in CMakeLists.txt:
```cmake
add_fuzzer(my_fuzzer my_fuzzer.cc base other_lib)
```

## Analyzing Results

During fuzzing, AFL++ saves all detected crashes in the `findings/crashes/` directory.
These files can be used to reproduce and fix the bugs. 