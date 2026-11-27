#include "cs2/update_manager.hpp"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <utility>
namespace cs2 { namespace {
bool valid_version(const std::string& v) {
 if(v.empty()) return false; bool digit=false;
 for(unsigned char c:v) { if(std::isdigit(c)) digit=true; else if(c=='.') { if(!digit)return false; digit=false; } else return false; }
 return digit;
}
bool valid_sha256(const std::string& s) { return s.size()==64&&std::all_of(s.begin(),s.end(),[](unsigned char c){return std::isxdigit(c)!=0;}); }
bool newer_version(const std::string& candidate,const std::string& current) {
 std::istringstream a(candidate),b(current); std::string x,y;
 for(;;) {
  const bool ax=static_cast<bool>(std::getline(a,x,'.')), by=static_cast<bool>(std::getline(b,y,'.'));
  if(!ax&&!by) return false; if(!ax)x="0"; if(!by)y="0";
  const auto nx=std::stoul(x),ny=std::stoul(y); if(nx!=ny)return nx>ny; x.clear();y.clear();
 }
}
}
std::string UpdateManifest::signed_payload() const {
 return product+"\n"+version+"\n"+channel+"\n"+artifact_url+"\n"+sha256+"\n"+minimum_version+"\n";
}
UpdateManager::UpdateManager(UpdateTransport& t,const UpdateTrust& trust,UpdateInstaller& i,std::string p,std::string v,std::string c)
 :transport_(t),trust_(trust),installer_(i),product_(std::move(p)),current_version_(std::move(v)),channel_(std::move(c)){}
UpdateResult UpdateManager::apply(const UpdateManifest& m) {
 if(m.product!=product_||m.channel!=channel_)return {false,"manifest product/channel mismatch"};
 if(!valid_version(m.version)||!valid_version(m.minimum_version)||!valid_version(current_version_))return {false,"invalid numeric version"};
 if(!newer_version(m.version,current_version_))return {false,"no newer version available"};
 if(newer_version(m.minimum_version,current_version_))return {false,"current version is below supported minimum"};
 if(m.artifact_url.rfind("https://",0)!=0)return {false,"updates require HTTPS"};
 if(!valid_sha256(m.sha256)||m.signature_base64.empty())return {false,"manifest integrity metadata is incomplete"};
 if(!trust_.verify_ed25519(m.signed_payload(),m.signature_base64))return {false,"publisher signature verification failed"};
 std::vector<unsigned char> package; std::string error;
 if(!transport_.download_https(m.artifact_url,package,error))return {false,"download failed: "+error};
 if(package.empty()||!trust_.verify_sha256(package,m.sha256))return {false,"package SHA-256 verification failed"};
 if(!installer_.stage_and_commit(package,error))return {false,"staged installation failed: "+error};
 return {true,"verified update installed"};
}
} // namespace cs2
