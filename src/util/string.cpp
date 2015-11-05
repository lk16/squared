#include "util/string.hpp"

std::string big_number(unsigned long long x){
//   std::stringstream ss;
//  
//   static const unsigned long long tenpowers[7] = {
//     1000000000000000000ull,
//     1000000000000000ull,
//     1000000000000ull,
//     1000000000ull,
//     1000000ull,
//     1000ull,
//     1ull
//   };
//   
//   bool first = true;
//   for(int i=0;i<7;++i){
//     if(x > tenpowers[i]){
//       unsigned long long tmp = x / tenpowers[i];
//       if(!first){
//         ss << std::setfill('0') << std::setw(3);
//       }
//       ss << tmp%1000;
//       if(i != 6){
//         ss << ',';
//       }
//       first = false;
//     }
//   }
//   
//   
//   return ss.str();
  return to_str<unsigned long long>(x);
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