// Copyright 2024
// Utility for analyzing fuzzing results

#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;

// Structure for storing statistics
struct FuzzStats {
  size_t total_files = 0;
  size_t crashes = 0;
  size_t timeouts = 0;
  size_t coverage = 0;
  
  std::unordered_map<int, size_t> result_codes;
};

// Function to analyze Redis protocol fuzzing results
FuzzStats analyzeRedisResults(const std::string& findings_dir) {
  FuzzStats stats;
  
  // Check if findings directory exists
  if (!fs::exists(findings_dir)) {
    std::cerr << "Redis findings directory does not exist: " << findings_dir << std::endl;
    return stats;
  }
  
  // Count crashes
  std::string crashes_dir = findings_dir + "/crashes";
  if (fs::exists(crashes_dir)) {
    for (const auto& entry : fs::directory_iterator(crashes_dir)) {
      stats.total_files++;
      stats.crashes++;
      
      // Read crash file and extract result code
      std::ifstream file(entry.path());
      std::string line;
      while (std::getline(file, line)) {
        if (line.find("Result code:") != std::string::npos) {
          int code = std::stoi(line.substr(line.find(":") + 1));
          stats.result_codes[code]++;
        }
      }
    }
  }
  
  // Count timeouts
  std::string timeouts_dir = findings_dir + "/hangs";
  if (fs::exists(timeouts_dir)) {
    for (const auto& entry : fs::directory_iterator(timeouts_dir)) {
      stats.total_files++;
      stats.timeouts++;
    }
  }
  
  // Read coverage information if available
  std::string plot_data = findings_dir + "/plot_data";
  if (fs::exists(plot_data)) {
    std::ifstream file(plot_data);
    std::string line;
    // Usually the last line contains the most recent stats
    while (std::getline(file, line)) {
      // Parse coverage from plot_data format
      // Format is usually: time, executions, ..., paths_total, ...
      auto pos = line.find(",");
      if (pos != std::string::npos) {
        pos = line.find(",", pos + 1);
        if (pos != std::string::npos) {
          pos = line.find(",", pos + 1);
          if (pos != std::string::npos) {
            std::string coverage_str = line.substr(pos + 1);
            pos = coverage_str.find(",");
            if (pos != std::string::npos) {
              coverage_str = coverage_str.substr(0, pos);
              stats.coverage = std::stoi(coverage_str);
            }
          }
        }
      }
    }
  }
  
  return stats;
}

// Function to analyze HTTP protocol fuzzing results
FuzzStats analyzeHttpResults(const std::string& findings_dir) {
  FuzzStats stats;
  
  // Check if findings directory exists
  if (!fs::exists(findings_dir)) {
    std::cerr << "HTTP findings directory does not exist: " << findings_dir << std::endl;
    return stats;
  }
  
  // Count crashes
  std::string crashes_dir = findings_dir + "/crashes";
  if (fs::exists(crashes_dir)) {
    for (const auto& entry : fs::directory_iterator(crashes_dir)) {
      stats.total_files++;
      stats.crashes++;
      
      // Read crash file and extract result code
      std::ifstream file(entry.path());
      std::string line;
      while (std::getline(file, line)) {
        if (line.find("Result code:") != std::string::npos) {
          int code = std::stoi(line.substr(line.find(":") + 1));
          stats.result_codes[code]++;
        }
      }
    }
  }
  
  // Count timeouts
  std::string timeouts_dir = findings_dir + "/hangs";
  if (fs::exists(timeouts_dir)) {
    for (const auto& entry : fs::directory_iterator(timeouts_dir)) {
      stats.total_files++;
      stats.timeouts++;
    }
  }
  
  // Read coverage information if available
  std::string plot_data = findings_dir + "/plot_data";
  if (fs::exists(plot_data)) {
    std::ifstream file(plot_data);
    std::string line;
    // Usually the last line contains the most recent stats
    while (std::getline(file, line)) {
      // Parse coverage from plot_data format
      auto pos = line.find(",");
      if (pos != std::string::npos) {
        pos = line.find(",", pos + 1);
        if (pos != std::string::npos) {
          pos = line.find(",", pos + 1);
          if (pos != std::string::npos) {
            std::string coverage_str = line.substr(pos + 1);
            pos = coverage_str.find(",");
            if (pos != std::string::npos) {
              coverage_str = coverage_str.substr(0, pos);
              stats.coverage = std::stoi(coverage_str);
            }
          }
        }
      }
    }
  }
  
  return stats;
}

// Print a summary of fuzzing results
void printSummary(const std::string& name, const FuzzStats& stats) {
  std::cout << "===== " << name << " Fuzzing Summary =====" << std::endl;
  std::cout << "Total files: " << stats.total_files << std::endl;
  std::cout << "Crashes: " << stats.crashes << std::endl;
  std::cout << "Timeouts: " << stats.timeouts << std::endl;
  std::cout << "Code coverage paths: " << stats.coverage << std::endl;
  
  std::cout << "Result code distribution:" << std::endl;
  for (const auto& [code, count] : stats.result_codes) {
    std::cout << "  Code " << code << ": " << count << " occurrences" << std::endl;
  }
  
  std::cout << std::endl;
}

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <redis_findings_dir> <http_findings_dir>" << std::endl;
    return 1;
  }
  
  std::string redis_dir = argv[1];
  std::string http_dir = argv[2];
  
  // Analyze Redis protocol fuzzing results
  FuzzStats redis_stats = analyzeRedisResults(redis_dir);
  printSummary("Redis Protocol", redis_stats);
  
  // Analyze HTTP protocol fuzzing results
  FuzzStats http_stats = analyzeHttpResults(http_dir);
  printSummary("HTTP Protocol", http_stats);
  
  return 0;
} 