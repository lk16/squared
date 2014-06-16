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


inline unsigned long long rand_64(){
  return ((unsigned long long) rand() << 32) | (unsigned long long) rand();
}

template<class T>
inline bool in_bounds(T x,T min,T max){
  return x>=min && x<=max;
}

template<class T>
inline bool out_bounds(T x,T min,T max){
  return x<min || x>max;        
}