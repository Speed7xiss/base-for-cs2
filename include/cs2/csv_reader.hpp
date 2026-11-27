#pragma once
#include <filesystem>
#include <string>
#include <vector>
namespace cs2 { using CsvRow=std::vector<std::string>; class CsvReader { public: static std::vector<CsvRow> read(const std::filesystem::path&); static std::vector<CsvRow> parse(const std::string&); }; }
