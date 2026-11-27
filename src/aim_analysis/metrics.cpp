#include "cs2/metrics.hpp"
#include <cmath>
#include <stdexcept>
namespace cs2 {
namespace {
float wrap(float d){while(d>180.0F)d-=360.0F;while(d< -180.0F)d+=360.0F;return d;}
float err(const AimSample& s){const float y=wrap(s.target_yaw-s.view_yaw);const float p=s.target_pitch-s.view_pitch;return std::sqrt(y*y+p*p);}
}
AimMetrics AimAnalyzer::analyze(const std::vector<AimSample>& samples,float threshold){
 if(threshold<0.0F||!std::isfinite(threshold))throw std::invalid_argument("Invalid threshold");
 AimMetrics out;out.samples=samples.size();if(samples.empty())return out;
 double sum=0.0,speed=0.0;std::size_t on=0,speed_count=0;std::vector<double> errors;errors.reserve(samples.size());
 for(std::size_t i=0;i<samples.size();++i){
  const double e=static_cast<double>(err(samples[i]));errors.push_back(e);sum+=e;if(e<=threshold)++on;
  if(i>0){if(samples[i].target_id!=samples[i-1].target_id)++out.target_switches;const double dt=samples[i].time_seconds-samples[i-1].time_seconds;
   if(dt>0.0){const float dy=wrap(samples[i].view_yaw-samples[i-1].view_yaw);const float dp=samples[i].view_pitch-samples[i-1].view_pitch;speed+=std::sqrt(static_cast<double>(dy*dy+dp*dp))/dt;++speed_count;}
  }
 }
 const double count=static_cast<double>(errors.size());out.mean_angular_error_degrees=sum/count;double variance=0.0;
 for(const double e:errors){const double delta=e-out.mean_angular_error_degrees;variance+=delta*delta;}
 out.angular_error_stddev_degrees=std::sqrt(variance/count);out.mean_angular_speed_deg_per_second=speed_count?speed/static_cast<double>(speed_count):0.0;out.tracking_coverage=static_cast<double>(on)/count;return out;
}
}
