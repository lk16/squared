#pragma once


#include <algorithm>
#include <cmath>
#include <cstdlib>

#include "bitset.hpp"

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
inline void ugly_sort(T *t,int* heurs, int count)
{
  bool loop;
  do{
    loop = false;
    for(int i=1;i<count;++i){
      if(heurs[i-1] < heurs[i]){
        std::swap(heurs[i-1],heurs[i]);
        std::swap(t[i-1],t[i]);
        loop = true;
      }
    }
  }while(loop);
}