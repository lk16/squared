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
inline bool in_bounds(T x,T min,T max){
  return x>=min && x<=max;
}

template<class T>
inline bool out_bounds(T x,T min,T max){
  return x<min || x>max;        
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

inline bits64 left_shift_internal(std::true_type,bits64 lhs,int rhs){
  return lhs << rhs; 
}

inline bits64 left_shift_internal(std::false_type,bits64 lhs,int rhs){
  return lhs >> (-rhs);
}


template<int rhs>
inline bits64 left_shift(bits64 lhs){
  constexpr bool is_positive_shift = (rhs > 0);
  typedef std::integral_constant<bool,is_positive_shift> is_positive_shift_t;
  return left_shift_internal(is_positive_shift_t(),lhs,rhs);
}

template<int rhs>
inline bits64 right_shift(bits64 lhs){
  constexpr bool is_negative_shift = (rhs < 0);
  typedef std::integral_constant<bool,is_negative_shift> is_negative_shift_t;
  return left_shift_internal(is_negative_shift_t(),lhs,-rhs);
}