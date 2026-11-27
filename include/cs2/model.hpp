#pragma once
#include <cstdint>
#include <string>
#include <vector>
namespace cs2 {
struct Vec2 { float x{}; float y{}; };
struct Segment2D { Vec2 from; Vec2 to; };
struct PlayerSnapshot { std::uint64_t steam_id{}; std::string name,team; int health{}; std::string weapon; float distance{}; Vec2 screen_top_left{},screen_size{}; std::vector<Segment2D> skeleton; };
struct AimSample { double time_seconds{}; float view_yaw{},view_pitch{},target_yaw{},target_pitch{}; std::string target_id; };
struct DemoFrame { double time_seconds{}; std::vector<PlayerSnapshot> players; };
struct DemoDataset { std::string source_label; std::vector<DemoFrame> frames; std::vector<AimSample> aim_samples; };
}
