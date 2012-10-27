#ifndef SQUARED_SHARED_HPP
#define SQUARED_SHARED_HPP

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>

#ifndef NDEBUG

#define SHOW_VAR(a) std::cout << __FILE__ << ':' << __LINE__ << '\t' << #a << " = " << (a) << std::endl 
#define SHOW_VAR2(a,b)     SHOW_VAR(a);      SHOW_VAR(b)
#define SHOW_VAR3(a,b,c)   SHOW_VAR2(a,b);   SHOW_VAR(c)
#define SHOW_VAR4(a,b,c,d) SHOW_VAR3(a,b,c); SHOW_VAR(d)
#define CONTAINS_BUG std::cout << __FILE__ << ':' << __LINE__ << '\t' << "Do not use this function, it contains bugs!\n"
#define PING std::cout << __FILE__ << ':' << __LINE__ << "\t PING!\n"
#define CRASH assert(false)

#else

#define SHOW_VAR(a)
#define SHOW_VAR2(a,b)
#define SHOW_VAR3(a,b,c)
#define SHOW_VAR4(a,b,c,d)
#define CONTAINS_BUG
#define PING

#endif



const std::string IMAGE_PATH = "./images/";
const std::string UI_PATH = "./xml/";



enum color{
  BLACK = 0,
  WHITE = 1,
  EMPTY = 2
};

inline color opponent(color c){
  assert(c==WHITE || c==BLACK);
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

template<class T>
std::string tostr(T x){
  std::stringstream buff;
  buff << x;
  return buff.str();
}

template<class T>
T fromstr(std::string x){
  std::stringstream buff(x);
  T temp;
  buff >> temp;
  return temp;
}

#endif