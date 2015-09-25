#include "main/testing.hpp"
  


void testing_area()
{
  bits64 x(rand_64());
  
  for(int i=0;i<8;++i){
    x.rotate(i).show();
  }
  
}

