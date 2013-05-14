#include "game/board.hpp"
#include "bots/bot_ali.hpp"

#include <ctime>
#include <iostream>

int main(){
  board b;
  bot_ali ali(BLACK,3,3);
  int start,diff,last,counter;
  
  start = std::time(NULL);
  counter = 0;
  b.reset();
  while(start==std::time(NULL));
  last = start = std::time(NULL);
  while(1){
    ali.heuristic(&b);
    counter++;
    if(last!=std::time(NULL)){
      last = std::time(NULL);
      diff = last-start;
      std::cout << counter << " / " << diff << " = " << (counter/diff) << " per second.\n";
    }
  }
  return 0;
}