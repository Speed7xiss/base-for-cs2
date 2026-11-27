#include "cs2/csv_reader.hpp"
#include "cs2/geometry.hpp"
#include "cs2/metrics.hpp"
#include "cs2/settings.hpp"
#include "cs2/schema_registry.hpp"
#include "cs2/update_manager.hpp"
#include <cmath>
#include <filesystem>
#include <iostream>
namespace {
int fails=0;
void check(bool b,const char* n){if(!b){std::cerr<<"FAIL "<<n<<'\n';++fails;}}
struct Transport final:cs2::UpdateTransport {
 bool download_https(const std::string&,std::vector<unsigned char>& out,std::string&) override {out={1,2,3};return true;}
};
struct Trust final:cs2::UpdateTrust {
 bool verify_ed25519(const std::string&,const std::string&) const override{return true;}
 bool verify_sha256(const std::vector<unsigned char>&,const std::string&) const override{return true;}
};
struct Installer final:cs2::UpdateInstaller {
 bool stage_and_commit(const std::vector<unsigned char>&,std::string&) override{return true;}
};
}
int main(){
 auto r=cs2::CsvReader::parse("a,b\n1,\"hello, world\"\n2,\"quote \"\"ok\"\"\"\n");
 check(r.size()==3,"csv rows");check(r[1][1]=="hello, world","quoted comma");check(r[2][1]=="quote \"ok\"","escaped quote");
 std::vector<cs2::AimSample> s{{0,0,0,1,0,"a"},{1,1,0,1,0,"a"},{2,2,0,2,0,"b"}};
 auto m=cs2::AimAnalyzer::analyze(s);check(m.samples==3,"sample count");check(m.target_switches==1,"target switches");check(m.tracking_coverage>0.99,"tracking coverage");
 cs2::PlayerSnapshot p;p.screen_top_left={10,20};p.screen_size={30,40};auto bounds=cs2::player_rect(p);check(bounds.max.x==40&&bounds.max.y==60,"player bounds");
 auto path=std::filesystem::temp_directory_path()/"cs2-review-settings.json";cs2::AppSettings c;c.visuals.boxes=false;c.visuals.fov_radius=123;cs2::SettingsStore::save(path,c);auto loaded=cs2::SettingsStore::load(path);check(!loaded.visuals.boxes,"settings bool");check(std::abs(loaded.visuals.fov_radius-123)<0.01F,"settings float");std::filesystem::remove(path);
 cs2::SchemaRegistry registry;cs2::ExportSchema schema{"demo-csv",1,{{"timestamp","time_seconds"},{"player","name"}}};registry.add(schema);
 check(registry.find("demo-csv",1)!=nullptr,"schema lookup");check(registry.validate_header(schema,{"time_seconds","name"}).valid,"schema valid header");check(!registry.validate_header(schema,{"time_seconds"}).valid,"schema missing header");
 Transport transport;Trust trust;Installer installer;cs2::UpdateManager updater(transport,trust,installer,"offline-review","1.0.0","stable");
 cs2::UpdateManifest manifest{"offline-review","1.1.0","stable","https://updates.example.invalid/release.zip",std::string(64,'a'),"signature","1.0.0"};
 check(updater.apply(manifest).installed,"updater accepts verified package");manifest.artifact_url="http://insecure.invalid/file";check(!updater.apply(manifest).installed,"updater rejects non-HTTPS");
 if(!fails)std::cout<<"all tests passed\n";return fails?1:0;
}
