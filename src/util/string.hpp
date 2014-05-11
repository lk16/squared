#pragma once

#include <sstream>
#include <string>
#include <vector>

template<class T>
inline std::string tostr(T x){
  std::stringstream buff;
  buff << x;
  return buff.str();
}

template<class T>
inline T fromstr(std::string x){
  std::stringstream buff(x);
  T temp;
  buff >> temp;
  return temp;
}

std::string big_number(long long x);

std::vector<std::string> str_explode(std::string s,char d);