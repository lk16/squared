#include "timing.hpp"



void timing_area(){
  
  struct timeval start,end;
  std::bitset<64> dummy;
  double time_diff;
  board b;
  
  const int runs = 1000000;
  
  gettimeofday(&start,NULL); 
  for(int i=0;i<runs;i++){ 
    b.reset();
    b.do_move(19,&dummy); 
  } 
  gettimeofday(&end,NULL); 
  time_diff = (end.tv_sec + (end.tv_usec / 1000000.0)) -
  (start.tv_sec + (start.tv_usec / 1000000.0)); 
  std::cout << "board::do_move():\t\t" << runs << " / " << time_diff; 
  std::cout << " = " << big_number(runs/time_diff) << " per sec avg\n"; 
  
  gettimeofday(&start,NULL); 
  for(int i=0;i<runs;i++){ 
    b.reset();
    b.do_move_experimental(19,&dummy); 
  } 
  gettimeofday(&end,NULL); 
  time_diff = (end.tv_sec + (end.tv_usec / 1000000.0)) -
  (start.tv_sec + (start.tv_usec / 1000000.0)); 
  std::cout << "board::do_move_experimental():\t" << runs << " / " << time_diff; 
  std::cout << " = " << big_number(runs/time_diff) << " per sec avg\n"; 
  
  gettimeofday(&start,NULL); 
  for(int i=0;i<runs;i++){ 
    b.reset();
    b.get_valid_moves(&dummy); 
  } 
  gettimeofday(&end,NULL); 
  time_diff = (end.tv_sec + (end.tv_usec / 1000000.0)) -
  (start.tv_sec + (start.tv_usec / 1000000.0)); 
  std::cout << "board::get_valid_moves():\t" << runs << " / " << time_diff; 
  std::cout << " = " << big_number(runs/time_diff) << " per sec avg\n"; 
  
}