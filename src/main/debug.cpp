#ifdef NDEBUG
#undef NDEBUG
#endif

#define TEST_COUNT (5000000)


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

void test_speed_ali_heuristic(board* ptr,int size){
  board b;
  bot_ali ali(BLACK,3,3,3);
  long long counter = 0;
  timeval start,end;
  double diff = 0.0;
  int dummy;
  
  gettimeofday(&start,NULL);
  while(counter < TEST_COUNT){
    counter++;
    
    /// ### ### ### ///
    dummy +=  ali.heuristic(ptr+(counter%size));
    /// ### ### ### ///
  }
  gettimeofday(&end,NULL);
  diff = get_time_diff(&start,&end);
  
  std::cout << "test_heuristic_speed: \t\t" << counter << '/' << diff << " = ";
  std::cout << (int)(counter/diff) << " per sec\n";
  
}

void test_speed_get_children(board* ptr,int size){
  board b[33];
  long long counter = 0;
  timeval start,end;
  double diff = 0.0;
  int dummy;
  
  gettimeofday(&start,NULL);
  while(counter < TEST_COUNT){
    counter++;
    // b.turn is not initialized
    
    /// ### ### ### ///
    ptr[counter%size].get_children(b,&dummy);
    /// ### ### ### ///
  }
  gettimeofday(&end,NULL);
  diff = get_time_diff(&start,&end);
  
  std::cout << "test_move_generator_speed:\t" << counter << '/' << diff << " = ";
  std::cout << (int)(counter/diff) << " per sec\n";
  
}

void test_speed_has_moves(board* ptr,int size){
  board b;
  long long counter = 0;
  timeval start,end;
  double diff = 0.0;
  bool dummy;
  
  gettimeofday(&start,NULL);
  while(counter < TEST_COUNT){
    counter++;
    
    /// ### ### ### ///
    dummy |= ptr[counter%size].has_moves();
    /// ### ### ### ///
  }
  gettimeofday(&end,NULL);
  diff = get_time_diff(&start,&end);
  
  std::cout << "test_speed_has_moves:\t\t" << counter << '/' << diff << " = ";
  std::cout << (int)(counter/diff) << " per sec\n";
  
}

void get_random_boards(board* ptr,int size){
  for(int i=0;i<size;i++){
    ptr[i].discs[WHITE] = ((unsigned long long)rand() << 32) | rand();  
    ptr[i].discs[BLACK] = (((unsigned long long)rand() << 32) | rand());
    ptr[i].discs[BLACK] &= (~ptr[i].discs[WHITE].to_ulong());  
    ptr[i].turn = (color)(i % 2);
  }
}

int main(){
  board b[1000];
  get_random_boards(b,1000);
  
  test_speed_has_moves(b,1000);
  test_speed_ali_heuristic(b,1000);
//  test_speed_get_children(b,1000); // segfaults?
  
  b[0].reset();
  b->show();
  bot_ali ali(BLACK,10,10,12);
  ali.do_move(b,b+33);
  (b+33)->show();
  return 0;
}
