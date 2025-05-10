# Fuzzing for Helio Project

This directory contains tools for fuzzing various components of the Helio project. Fuzzing is used to detect security vulnerabilities, data processing errors, and other issues by feeding random or mutated input data.

## Required Dependencies

Fuzzing requires [AFL++](https://github.com/AFLplusplus/AFLplusplus). Install it according to the [instructions from the repository](https://github.com/AFLplusplus/AFLplusplus).

## Available Fuzzers

1. **flit_fuzzer** - fuzzer for Parse64Safe and Parse64Fast functions from base/flit.h
2. **redis_fuzzer** - fuzzer for Redis protocol analysis
3. **http_fuzzer** - fuzzer for HTTP protocol analysis

## Compilation and Running

To compile the fuzzers, include the ENABLE_FUZZING option during CMake configuration:

```bash
cmake -DENABLE_FUZZING=ON -B build .
cd build
make
```

### Running Fuzzing

To run fuzzing for a specific fuzzer:

```bash
cd build
make run_flit_fuzzer
```

For Redis or HTTP protocols:

```bash
make run_redis_fuzzer
make run_http_fuzzer
```

## Analyzing Results

After running fuzzing, results will be stored in the following directories:
- `build/fuzzing/flit_fuzzer_findings/`
- `build/fuzzing/redis_fuzzer_findings/`
- `build/fuzzing/http_fuzzer_findings/`

To analyze results manually, check these directories, particularly the `crashes/` and `hangs/` subdirectories.

### Automated Analysis

The following targets are available for analyzing results:

1. **Quick View**:
   ```bash
   make analyze_redis   # Analyze Redis fuzzer results
   make analyze_http    # Analyze HTTP fuzzer results
   make analyze_results # Analyze results from both fuzzers
   ```

2. **Detailed Analysis**:
   ```bash
   make detailed_analysis
   ```
   This command runs the `fuzzing_analyzer` utility, which processes results and provides detailed statistics.

## Input Data Formats

### Redis Protocol

Redis uses the RESP (Redis Serialization Protocol) text protocol with the following formats:
- Simple strings: `+OK\r\n`
- Errors: `-Error message\r\n`
- Integers: `:1000\r\n`
- Bulk strings: `$5\r\nHello\r\n`
- Arrays: `*2\r\n$3\r\nGET\r\n$3\r\nkey\r\n`

### HTTP Protocol

HTTP uses a text protocol with the following structure:
- Request line: `GET /path HTTP/1.1\r\n`
- Headers: `Host: example.com\r\n`
- Empty line: `\r\n`
- Body (optional): `param=value`

## Adding New Fuzzers

To add a new fuzzer:

1. Create a file with implementation (`your_fuzzer.cc`)
2. Add it to `fuzzing/CMakeLists.txt`:
   ```cmake
   add_fuzzer(your_fuzzer your_fuzzer.cc base)
   ```
3. Optionally update the results analysis utility and add new targets for analysis

## Extending the Results Analyzer

The `fuzzing_analyzer` utility is designed to analyze fuzzing results. To add support for analyzing a new fuzzer, add the corresponding function to `analysis.cc`. 