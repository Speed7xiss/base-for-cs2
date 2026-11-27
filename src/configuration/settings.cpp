#include "cs2/settings.hpp"
#include <fstream>
#include <iomanip>
#include <regex>
#include <stdexcept>
namespace cs2 { namespace {
std::string val(const std::string& s,const std::string& k){std::regex re("\\\""+k+"\\\"\\s*:\\s*(\\\"[^\\\"]*\\\"|true|false|[-+]?[0-9]*\\.?[0-9]+)");std::smatch m;if(!std::regex_search(s,m,re))return {};auto v=m[1].str();if(v.size()>1&&v.front()=='"')v=v.substr(1,v.size()-2);return v;}
bool boolean(const std::string& s,bool d){return s.empty()?d:s=="true";}float real(const std::string& s,float d){if(s.empty())return d;try{return std::stof(s);}catch(...){return d;}}
}}
AppSettings SettingsStore::load(const std::filesystem::path& p){std::ifstream f(p);if(!f)return {};std::string s((std::istreambuf_iterator<char>(f)),{});AppSettings a;a.visuals.boxes=boolean(val(s,"boxes"),a.visuals.boxes);a.visuals.skeleton=boolean(val(s,"skeleton"),a.visuals.skeleton);a.visuals.health=boolean(val(s,"health"),a.visuals.health);a.visuals.names=boolean(val(s,"names"),a.visuals.names);a.visuals.weapons=boolean(val(s,"weapons"),a.visuals.weapons);a.visuals.distance=boolean(val(s,"distance"),a.visuals.distance);a.visuals.team_colors=boolean(val(s,"team_colors"),a.visuals.team_colors);a.visuals.fov_radius=real(val(s,"fov_radius"),a.visuals.fov_radius);a.aim_error_threshold=real(val(s,"aim_error_threshold"),a.aim_error_threshold);auto n=val(s,"profile");if(!n.empty())a.profile=n;return a;}
void SettingsStore::save(const std::filesystem::path& p,const AppSettings& a){if(p.has_parent_path())std::filesystem::create_directories(p.parent_path());auto tmp=p.string()+".tmp";std::ofstream f(tmp);if(!f)throw std::runtime_error("Cannot write settings");f<<std::boolalpha<<std::fixed<<std::setprecision(2)<<"{\n  \"profile\": \""<<a.profile<<"\",\n  \"boxes\": "<<a.visuals.boxes<<",\n  \"skeleton\": "<<a.visuals.skeleton<<",\n  \"health\": "<<a.visuals.health<<",\n  \"names\": "<<a.visuals.names<<",\n  \"weapons\": "<<a.visuals.weapons<<",\n  \"distance\": "<<a.visuals.distance<<",\n  \"team_colors\": "<<a.visuals.team_colors<<",\n  \"fov_radius\": "<<a.visuals.fov_radius<<",\n  \"aim_error_threshold\": "<<a.aim_error_threshold<<"\n}\n";f.close();if(!f)throw std::runtime_error("Failed to flush settings");std::error_code ec;std::filesystem::rename(tmp,p,ec);if(ec)throw std::runtime_error("Failed to replace settings: "+ec.message());}
}
