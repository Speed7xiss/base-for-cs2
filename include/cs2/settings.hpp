#pragma once
#include <filesystem>
#include <string>
namespace cs2 { struct VisualSettings { bool boxes{true},skeleton{true},health{true},names{true},weapons{true},distance{true},team_colors{true}; float fov_radius{90}; }; struct AppSettings { VisualSettings visuals; float aim_error_threshold{2}; std::string profile{"default"}; }; class SettingsStore { public: static AppSettings load(const std::filesystem::path&); static void save(const std::filesystem::path&,const AppSettings&); }; }
