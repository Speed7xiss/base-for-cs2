#include "cs2/geometry.hpp"
namespace cs2 {Rect2D player_rect(const PlayerSnapshot& p){return {p.screen_top_left,{p.screen_top_left.x+p.screen_size.x,p.screen_top_left.y+p.screen_size.y}};}std::array<Vec2,4> rect_corners(const Rect2D& r){return {{{r.min.x,r.min.y},{r.max.x,r.min.y},{r.max.x,r.max.y},{r.min.x,r.max.y}}};}}
