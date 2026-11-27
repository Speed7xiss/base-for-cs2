#include "cs2/schema_registry.hpp"
#include <set>
#include <stdexcept>
#include <utility>
namespace cs2 {
void SchemaRegistry::add(ExportSchema schema) {
 if(schema.id.empty()||schema.version==0||schema.columns.empty()) throw std::invalid_argument("schema needs id, positive version and columns");
 const auto key=schema.id+"@"+std::to_string(schema.version);
 if(schemas_.contains(key)) throw std::invalid_argument("duplicate schema version: "+key);
 schemas_.emplace(key,std::move(schema));
}
const ExportSchema* SchemaRegistry::find(const std::string& id,unsigned version) const noexcept {
 const auto it=schemas_.find(id+"@"+std::to_string(version)); return it==schemas_.end()?nullptr:&it->second;
}
SchemaValidation SchemaRegistry::validate_header(const ExportSchema& schema,const std::vector<std::string>& header) const {
 SchemaValidation r; std::set<std::string> seen;
 for(const auto& n:header) if(!seen.insert(n).second) r.errors.push_back("duplicate CSV column: "+n);
 for(const auto& [logical,column]:schema.columns) if(!seen.contains(column)) r.errors.push_back("missing column '"+column+"' for field '"+logical+"'");
 r.valid=r.errors.empty(); return r;
}
} // namespace cs2
