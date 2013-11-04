#include "testing.hpp"

void testing_area(){
  for(int i=0;i<8;i++){
    show_bitset(board::walk_possible[i][0]);
  }
}
