#include "util/pgn.hpp"

pgn::pgn(const std::string& file_name)
{
  std::ifstream file(file_name);
  const int line_size = 1024;
  char line[line_size];
  
  while(file.good()){
    file.getline(line,line_size);
    if(line[0] == '['){
      std::string line_str(line);
      size_t key_end = line_str.find_first_of(' ',0);
      std::string key = line_str.substr(1,key_end-1);
      size_t value_start = line_str.find_first_of('\"',key_end-1) + 1;
      size_t value_end = line_str.find_first_of('\"',value_start+1);
      std::string value = line_str.substr(value_start,value_end-value_start);
      data[key] = value;
    }
    else{
      for(std::string& s: str_explode(line,' ')){
        int move = board::position_to_index(s);
        if(move >= 0){
          moves.push_back(move);
        }
      }
    }
  }
}



std::string pgn::get_data(const std::string& key)
{
  auto it = data.find(key);
  return (it == data.end()) ? "not found" : it->second;
}
