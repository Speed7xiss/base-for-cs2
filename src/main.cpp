#include "cs2/demo_dataset.hpp"
#include "cs2/metrics.hpp"
#include <exception>
#include <iomanip>
#include <iostream>
int main(int argc,char** argv){if(argc!=3){std::cout<<"CS2 Offline Review 0.1.0\nUsage: cs2_review <players.csv> <aim.csv>\n";return 0;}try{auto d=cs2::DemoDatasetLoader::load(argv[1],argv[2]);auto m=cs2::AimAnalyzer::analyze(d.aim_samples);std::size_t n=0;for(const auto& f:d.frames)n+=f.players.size();std::cout<<"Source: "<<d.source_label<<"\nFrames: "<<d.frames.size()<<"\nPlayer snapshots: "<<n<<"\nAim samples: "<<m.samples<<"\nTarget switches: "<<m.target_switches<<std::fixed<<std::setprecision(2)<<"\nMean angular error: "<<m.mean_angular_error_degrees<<" deg\nError stddev: "<<m.angular_error_stddev_degrees<<" deg\nMean angular speed: "<<m.mean_angular_speed_deg_per_second<<" deg/s\nTracking coverage: "<<m.tracking_coverage*100<<"%\n";return 0;}catch(const std::exception& e){std::cerr<<"error: "<<e.what()<<'\n';return 1;}}
