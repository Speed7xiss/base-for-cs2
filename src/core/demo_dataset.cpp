#include "cs2/demo_dataset.hpp"
#include "cs2/csv_reader.hpp"
#include <charconv>
#include <cmath>
#include <stdexcept>
#include <string_view>
#include <unordered_map>
namespace cs2 { namespace {
float num(std::string_view s,const char* k){float v{};auto r=std::from_chars(s.data(),s.data()+s.size(),v);if(r.ec!=std::errc{}||r.ptr!=s.data()+s.size()||!std::isfinite(v))throw std::runtime_error(std::string("Invalid field: ")+k);return v;}
std::uint64_t uid(std::string_view s){std::uint64_t v{};auto r=std::from_chars(s.data(),s.data()+s.size(),v);if(r.ec!=std::errc{}||r.ptr!=s.data()+s.size())throw std::runtime_error("Invalid steam_id");return v;}
std::unordered_map<std::string,std::size_t> hdr(const CsvRow& r){std::unordered_map<std::string,std::size_t> h;for(std::size_t i=0;i<r.size();++i)h.emplace(r[i],i);return h;}
const std::string& col(const CsvRow& r,const std::unordered_map<std::string,std::size_t>& h,const char* k){auto i=h.find(k);if(i==h.end()||i->second>=r.size())throw std::runtime_error(std::string("Missing column: ")+k);return r[i->second];}
std::vector<Segment2D> bones(const std::string& s){std::vector<Segment2D> o;std::size_t b=0;while(b<s.size()){auto e=s.find(';',b),m=s.find('|',b);if(m==std::string::npos||(e!=std::string::npos&&m>e))throw std::runtime_error("Invalid bones");auto point=[](std::string_view p){auto c=p.find(':');if(c==std::string_view::npos)throw std::runtime_error("Invalid bone point");return Vec2{num(p.substr(0,c),"bone_x"),num(p.substr(c+1),"bone_y")};};o.push_back({point(std::string_view(s).substr(b,m-b)),point(std::string_view(s).substr(m+1,(e==std::string::npos?s.size():e)-m-1))});if(e==std::string::npos)break;b=e+1;}return o;}
}}
DemoDataset DemoDatasetLoader::load(const std::filesystem::path& pp,const std::filesystem::path& ap){DemoDataset d;d.source_label=pp.filename().string();auto pr=CsvReader::read(pp);if(pr.size()<2)throw std::runtime_error("Player CSV has no data");auto ph=hdr(pr[0]);for(std::size_t i=1;i<pr.size();++i){const auto& r=pr[i];double t=num(col(r,ph,"time_seconds"),"time_seconds");if(d.frames.empty()||std::abs(d.frames.back().time_seconds-t)>0.0001)d.frames.push_back({t,{}});PlayerSnapshot p;p.steam_id=uid(col(r,ph,"steam_id"));p.name=col(r,ph,"name");p.team=col(r,ph,"team");p.health=static_cast<int>(num(col(r,ph,"health"),"health"));if(p.health<0||p.health>100)throw std::runtime_error("health out of range");p.weapon=col(r,ph,"weapon");p.distance=num(col(r,ph,"distance"),"distance");p.screen_top_left={num(col(r,ph,"screen_x"),"screen_x"),num(col(r,ph,"screen_y"),"screen_y")};p.screen_size={num(col(r,ph,"screen_w"),"screen_w"),num(col(r,ph,"screen_h"),"screen_h")};auto b=ph.find("bones");if(b!=ph.end()&&b->second<r.size()&&!r[b->second].empty())p.skeleton=bones(r[b->second]);d.frames.back().players.push_back(std::move(p));}
auto ar=CsvReader::read(ap);if(ar.size()<2)throw std::runtime_error("Aim CSV has no data");auto ah=hdr(ar[0]);for(std::size_t i=1;i<ar.size();++i){auto& r=ar[i];d.aim_samples.push_back({static_cast<double>(num(col(r,ah,"time_seconds"),"time_seconds")),num(col(r,ah,"view_yaw"),"view_yaw"),num(col(r,ah,"view_pitch"),"view_pitch"),num(col(r,ah,"target_yaw"),"target_yaw"),num(col(r,ah,"target_pitch"),"target_pitch"),col(r,ah,"target_id")});}return d;}
}
