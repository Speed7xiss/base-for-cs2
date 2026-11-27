#pragma once
#include <string>
#include <vector>
namespace cs2 {
struct UpdateManifest {
 std::string product,version,channel,artifact_url,sha256,signature_base64,minimum_version;
 [[nodiscard]] std::string signed_payload() const;
};
// Production adapters MUST validate TLS, pin an Ed25519 publisher key, and use real SHA-256.
class UpdateTransport { public: virtual ~UpdateTransport()=default; virtual bool download_https(const std::string&,std::vector<unsigned char>&,std::string&)=0; };
class UpdateTrust {
public: virtual ~UpdateTrust()=default;
 virtual bool verify_ed25519(const std::string&,const std::string&) const=0;
 virtual bool verify_sha256(const std::vector<unsigned char>&,const std::string&) const=0;
};
class UpdateInstaller {
public: virtual ~UpdateInstaller()=default;
 // Stage, validate and atomically switch; preserve rollback on failure.
 virtual bool stage_and_commit(const std::vector<unsigned char>&,std::string&)=0;
};
struct UpdateResult { bool installed{false}; std::string message; };
class UpdateManager {
public:
 UpdateManager(UpdateTransport&,const UpdateTrust&,UpdateInstaller&,std::string product,std::string current_version,std::string channel);
 [[nodiscard]] UpdateResult apply(const UpdateManifest&);
private:
 UpdateTransport& transport_; const UpdateTrust& trust_; UpdateInstaller& installer_;
 std::string product_,current_version_,channel_;
};
} // namespace cs2
