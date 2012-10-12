#ifndef SQUARED_SHARED_HPP
#define SQUARED_SHARED_HPP

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>

#ifndef NDEBUG
#define SHOW_VAR(var) std::cout << __FILE__ << ':' << __LINE__ << '\t' << #var << " = " << (var) << std::endl 
#else
#define SHOW_VAR(var)
#endif



const std::string IMAGE_PATH = "./images/";
const std::string UI_PATH = "./ui/";



enum color{
  BLACK = 0,
  WHITE = 1,
  EMPTY = 2
};

inline color opponent(color c){
  assert(c!=EMPTY);
  return ((c==BLACK) ? WHITE : BLACK);
}

template<class T>
inline T min(T x,T y){
  return ((x<y) ? x : y);
}

template<class T>
inline T max(T x,T y){
  return ((x>y) ? x : y);
}


template<class T>
inline T abs(T x){
  return ((x<0) ? x : (-x));
}



#endif