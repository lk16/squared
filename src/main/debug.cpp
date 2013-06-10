#ifdef NDEBUG
#undef NDEBUG
#endif

#include "game/util.hpp"
#include "game/board.hpp"
#include "bots/bot_ali.hpp"
#include "debug/dbg_heur_algo.hpp"

#include <ctime>
#include <iostream>
#include <algorithm>
#include <sys/time.h>

void do_random_moves(const board* b,int n,board* result){
  board m[32];
  int count;
  for(int i=n-1;i>=0;--i){
    b->get_children(m,&count);
    if(count == 0){
      board copy(*b);
      copy.turn = opponent(copy.turn);
      copy.get_children(m,&count);
      if(count == 0){
        return;
      }
    }
    *result = m[rand()%count]; 
  }
} 

double get_time_diff(timeval start,timeval end){
  return (end.tv_sec + (end.tv_usec / 1000000.0))
  - (start.tv_sec + (start.tv_usec / 1000000.0));
}


void test_heur_algos_validity(){
  board b;
  long long counter = 0;
  while(true){
    b.reset();
    do_random_moves(&b,1+(rand()%55),&b);
    
    board buff[32];
    int child_count;
    b.get_children(buff,&child_count);
    if(child_count == 0){
      b.turn = opponent(b.turn);
    }
    b.get_children(buff,&child_count);
    if(child_count == 0){
      continue; // one color has been swiped off the board
    }
    
    int depth = 1 + (rand() % 4);
    
    int sign = (b.turn==WHITE ? 1 : -1);
    assert(       minimax_dbg(&b,depth) == alpha_beta_dbg(&b,-99999,99999,depth));
    assert(sign * minimax_dbg(&b,depth) == negamax_dbg(&b,-99999,99999,depth));
    
    counter++;
    if(counter % 1000 == 0){
      std::cout << counter << std::endl;
    }
  }
}

int main(){
  test_heur_algos_validity();
  return 0;
}
