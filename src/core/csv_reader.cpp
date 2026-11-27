#include "cs2/csv_reader.hpp"
#include <fstream>
#include <stdexcept>
namespace cs2 {
std::vector<CsvRow> CsvReader::parse(const std::string& in){std::vector<CsvRow> rows;CsvRow row;std::string field;bool q=false;for(std::size_t i=0;i<in.size();++i){char c=in[i];if(q){if(c=='"'&&i+1<in.size()&&in[i+1]=='"'){field.push_back('"');++i;}else if(c=='"')q=false;else field.push_back(c);}else if(c=='"'&&field.empty())q=true;else if(c==','){row.push_back(std::move(field));field.clear();}else if(c=='\n'){if(!field.empty()&&field.back()=='\r')field.pop_back();row.push_back(std::move(field));field.clear();if(!(row.size()==1&&row[0].empty()))rows.push_back(std::move(row));row.clear();}else field.push_back(c);}if(q)throw std::runtime_error("Unterminated CSV quote");if(!field.empty()||!row.empty()){row.push_back(std::move(field));rows.push_back(std::move(row));}return rows;}
std::vector<CsvRow> CsvReader::read(const std::filesystem::path& p){std::ifstream f(p,std::ios::binary);if(!f)throw std::runtime_error("Cannot open CSV: "+p.string());return parse(std::string((std::istreambuf_iterator<char>(f)),{}));}
}
