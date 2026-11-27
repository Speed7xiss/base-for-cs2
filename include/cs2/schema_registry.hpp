#pragma once
#include <map>
#include <string>
#include <vector>
namespace cs2 {
struct ExportSchema { std::string id; unsigned version{1}; std::map<std::string,std::string> columns; };
struct SchemaValidation { bool valid{false}; std::vector<std::string> errors; };
// Maps logical fields to CSV header names in authorized OFFLINE exports.
// These are not process-memory offsets or live-game addresses.
class SchemaRegistry {
public:
 void add(ExportSchema schema);
 [[nodiscard]] const ExportSchema* find(const std::string& id,unsigned version) const noexcept;
 [[nodiscard]] SchemaValidation validate_header(const ExportSchema&,const std::vector<std::string>&) const;
private: std::map<std::string,ExportSchema> schemas_;
};
} // namespace cs2
