#include "util/string.hpp"

std::string big_number(long long x){
  std::stringstream ss;
  if(x>10000000000l){
    x /= 1000000000l;
    ss << x << 'G'; 
  }
  else if(x>10000000l){
    x /= 1000000l;
    ss << x << 'M';
  }
  else if(x>1000l){
    x /= 1000l;
    ss << x << 'K';
  }
  else{
    ss << x;
  }
  
  return ss.str();
}

std::vector<std::string> str_explode(std::string s,char d){
  size_t start=0,end;
  std::vector<std::string> res;
  while(true){
    end = s.find_first_of(d,start);
    if(end == std::string::npos){
      res.push_back(s.substr(start));
      break;
    }
    res.push_back(s.substr(start,end-start));
    start = end+1;
  }
  return res;
}