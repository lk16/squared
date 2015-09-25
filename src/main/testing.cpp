#include "main/testing.hpp"
  


void testing_area()
{
  bits64 transformed[7][256];
  // 0: left 
  // 1: right 
  // 2: 180 
  // 3: mirror vertically 
  // 4: mirror vertically + right
  // 5: mirror vertically + 180
  // 6: mirror vertically + left
  
  for(unsigned long i=0;i<256;++i){
    bits64 tmp;
    
    tmp = i;
    transformed[0][i] = 0ull;
    for(unsigned j=0;j<8;++j){
      if(tmp.test(j)){
        transformed[0][i].set(8*(7-j));
        transformed[1][i].set(7+8*j);
        transformed[2][i].set(63-j);
        transformed[3][i].set(7-j);
        transformed[4][i].set(63-8*j);
        transformed[5][i].set(56+j);
        transformed[6][i].set(8*j);
      }
    }
  } 
  
  
  std::cout << "{\n";
  for(int r=0;r<7;++r){
    std::cout << "{\n";
    for(int x=0;x<256;++x){
      std::cout << transformed[r][x].get_word() << "ull";
      if(x!=255){
        std::cout << ",\n";
      }
      else{
        std::cout << '\n';
      }
    }
    if(r==6){
      std::cout << "}\n";
    }
    else{
      std::cout << "},\n";
    }
  }
  std::cout << "}\n";
}

