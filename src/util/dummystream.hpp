#pragma once

#include <ostream>

struct dummystream:
  public std::ostream
{
  
  template<class T>
  dummystream& operator<<(T t){
    (void)t;
    return *this;
  }
  
};