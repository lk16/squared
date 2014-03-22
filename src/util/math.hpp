#pragma once

#include <cstdlib>

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


inline unsigned long rand_64(){
  return ((unsigned long) rand() << 32) | (unsigned long) rand();
}