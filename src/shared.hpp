#ifndef SQUARED_SHARED_HPP
#define SQUARED_SHARED_HPP

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>

#define LOCATION __FILE__ << ':' << __LINE__ << '\t'

#ifndef NDEBUG

  #define SHOW_VAR(a) std::cout << LOCATION << #a << " = " << (a) << std::endl 
  #define SHOW_VAR2(a,b)     SHOW_VAR(a);      SHOW_VAR(b)
  #define SHOW_VAR3(a,b,c)   SHOW_VAR2(a,b);   SHOW_VAR(c)
  #define SHOW_VAR4(a,b,c,d) SHOW_VAR3(a,b,c); SHOW_VAR(d)
  #define PING std::cout << LOCATION << "PING!\n"
  #define CONTAINS_BUG std::cout << LOCATION << "Do not use this function, it contains bugs!\n"
  #define NOT_IMPLEMENTED std::cout << LOCATION << "This is not implemented yet!\n" 
  #define CRASH std::cout << LOCATION << "I'm crashing!\n"; abort()

#else

  #define SHOW_VAR(a)
  #define SHOW_VAR2(a,b)
  #define SHOW_VAR3(a,b,c)
  #define SHOW_VAR4(a,b,c,d)
  #define PING
  #define CONTAINS_BUG
  #define NOT_IMPLEMENTED
  #define CRASH
  
#endif

/// subdirectory where images are stored
const std::string IMAGE_PATH = "./images/";

/// subdirectory where xml files containing ui are stored
const std::string UI_PATH = "./xml/";

/// size of othello field, assuming width==height
const int FIELD_SIZE = 8;

/// total number of fields 
const int TOTAL_FIELDS = FIELD_SIZE*FIELD_SIZE;

/// heighest possible heuristic value
const int MAX_HEURISTIC =  100 * TOTAL_FIELDS;

/// lowest possible heuristic value
const int MIN_HEURISTIC = -100 * TOTAL_FIELDS;

enum color{
  BLACK = 0,
  WHITE = 1,
  EMPTY = 2
};

enum bot_state{
  BOT_STATE_NOT_STARTED,
  BOT_STATE_CALCULATING,
  BOT_STATE_FINISHED  
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