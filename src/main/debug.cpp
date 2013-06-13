#ifdef NDEBUG
#undef NDEBUG
#endif

#define MILLION (1000000)
#define BILLION (1000 * MILLION)


#include "game/util.hpp"
#include "game/board.hpp"
#include "bots/bot_ali.hpp"

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

double get_time_diff(timeval* start,timeval* end){
  return (end->tv_sec + (end->tv_usec / 1000000.0))
  - (start->tv_sec + (start->tv_usec / 1000000.0));
}

void test_speed_ali_heuristic(){
  board b;
  bot_ali ali(BLACK,3,3);
  long long counter = 0;
  timeval start,end;
  double diff = 0.0;
  int dummy;
  
  gettimeofday(&start,NULL);
  while(counter < MILLION){
    counter++;
    b.discs[WHITE] = ((unsigned long long)rand() << 32) | rand();  
    b.discs[BLACK] = (((unsigned long long)rand() << 32) | rand());
    b.discs[BLACK] &= (~b.discs[WHITE].to_ulong());  
    // b.turn is not initialized
    
    /// ### ### ### ///
    dummy +=  ali.heuristic(&b);
    /// ### ### ### ///
  }
  gettimeofday(&end,NULL);
  diff = get_time_diff(&start,&end);
  
  std::cout << "test_heuristic_speed: \t\t" << counter << '/' << diff << " = ";
  std::cout << (int)(counter/diff) << " per sec\n";
  
}

void test_speed_get_children(){
  board b[33];
  long long counter = 0;
  timeval start,end;
  double diff = 0.0;
  int dummy;
  
  gettimeofday(&start,NULL);
  while(counter < MILLION){
    counter++;
    b[0].discs[WHITE] = ((unsigned long long)rand() << 32) | rand();  
    b[0].discs[BLACK] = (((unsigned long long)rand() << 32) | rand());
    b[0].discs[BLACK] &= (~b[0].discs[WHITE].to_ulong());  
    // b.turn is not initialized
    
    /// ### ### ### ///
    b[0].get_children(b+1,&dummy);
    /// ### ### ### ///
  }
  gettimeofday(&end,NULL);
  diff = get_time_diff(&start,&end);
  
  std::cout << "test_move_generator_speed:\t" << counter << '/' << diff << " = ";
  std::cout << (int)(counter/diff) << " per sec\n";
  
}

void test_speed_has_moves(){
  board b;
  long long counter = 0;
  timeval start,end;
  double diff = 0.0;
  bool dummy;
  
  gettimeofday(&start,NULL);
  while(counter < MILLION){
    counter++;
    b.discs[WHITE] = ((unsigned long long)rand() << 32) | rand();  
    b.discs[BLACK] = (((unsigned long long)rand() << 32) | rand());
    b.discs[BLACK] &= (~b.discs[WHITE].to_ulong());  
    b.turn = (color)(counter % 2);
    
    /// ### ### ### ///
    dummy |= b.has_moves();
    /// ### ### ### ///
  }
  gettimeofday(&end,NULL);
  diff = get_time_diff(&start,&end);
  
  std::cout << "test_speed_has_moves:\t\t" << counter << '/' << diff << " = ";
  std::cout << (int)(counter/diff) << " per sec\n";
  
}

int main(){
  test_speed_has_moves();
  test_speed_ali_heuristic();
  test_speed_get_children();
  return 0;
}
