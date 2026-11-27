#pragma once
#include "cs2/model.hpp"
#include <filesystem>
namespace cs2 { class DemoDatasetLoader { public: static DemoDataset load(const std::filesystem::path& players_csv,const std::filesystem::path& aim_csv); }; }
