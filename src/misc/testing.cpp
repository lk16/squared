#include "testing.hpp"


void testing_area(){
  board b;
  b.turn = b.passed = false;
  b.me = 0x3;
  b.me = rotate_left(b.me);
  b.me = mirror_vertical_line(b.me);
  b.show();
}
