#pragma once


#include <cstdio>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "math.hpp"

template<class T>
inline std::string to_str(T x){
  std::stringstream buff;
  buff << x;
  return buff.str();
}

template<class T>
inline T from_str(std::string x){
  std::stringstream buff(x);
  T temp;
  buff >> temp;
  return temp;
}

std::string big_number(unsigned long long x);

std::vector<std::string> str_explode(std::string s,char d);

template<class T>
inline bool string_convertible_to(const std::string& x){
  std::stringstream buff(x);
  T temp;
  buff >> temp;
  return buff.fail();
}