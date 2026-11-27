#include "cs2/csv_reader.hpp"
#include "cs2/geometry.hpp"
#include "cs2/metrics.hpp"
#include "cs2/settings.hpp"
#include <cmath>
#include <filesystem>
#include <iostream>
namespace {int fails=0;void check(bool b,const char* n){if(!b){std::cerr<<"FAIL "<<n<<'\n';++fails;}}}
int main(){auto r=cs2::CsvReader::parse("a,b\n1,\"hello, world\"\n2,\"quote \"\"ok\"\"\"\n");check(r.size()==3,"csv rows");check(r[1][1]=="hello, world","quoted comma");check(r[2][1]=="quote \"ok\"","escaped quote");std::vector<cs2::AimSample> s{{0,0,0,1,0,"a"},{1,1,0,1,0,"a"},{2,2,0,2,0,"b"}};auto m=cs2::AimAnalyzer::analyze(s);check(m.samples==3,"sample count");check(m.target_switches==1,"target switches");check(m.tracking_coverage>0.99,"tracking coverage");cs2::PlayerSnapshot p;p.screen_top_left={10,20};p.screen_size={30,40};auto b=cs2::player_rect(p);check(b.max.x==40&&b.max.y==60,"player bounds");auto path=std::filesystem::temp_directory_path()/"cs2-review-settings.json";cs2::AppSettings c;c.visuals.boxes=false;c.visuals.fov_radius=123;cs2::SettingsStore::save(path,c);auto loaded=cs2::SettingsStore::load(path);check(!loaded.visuals.boxes,"settings bool");check(std::abs(loaded.visuals.fov_radius-123)<0.01F,"settings float");std::filesystem::remove(path);if(!fails)std::cout<<"all tests passed\n";return fails?1:0;}
