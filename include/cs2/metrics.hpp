#pragma once
#include "cs2/model.hpp"
#include <cstddef>
namespace cs2 { struct AimMetrics { std::size_t samples{},target_switches{}; double mean_angular_error_degrees{},angular_error_stddev_degrees{},mean_angular_speed_deg_per_second{},tracking_coverage{}; }; class AimAnalyzer { public: static AimMetrics analyze(const std::vector<AimSample>&,float on_target_threshold_degrees=2.0F); }; }
