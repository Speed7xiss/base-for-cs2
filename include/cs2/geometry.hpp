#pragma once
#include "cs2/model.hpp"
#include <array>
namespace cs2 { struct Rect2D { Vec2 min,max; }; Rect2D player_rect(const PlayerSnapshot&); std::array<Vec2,4> rect_corners(const Rect2D&); }
