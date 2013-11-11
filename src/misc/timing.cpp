#include "timing.hpp"

void print_diff(timeval start,timeval end,int runs,const char* name){
  
  double time_diff = (end.tv_sec + (end.tv_usec / 1000000.0)) -
  (start.tv_sec + (start.tv_usec / 1000000.0)); 
  std::cout << name << '\t' << runs << " / " << time_diff; 
  std::cout << " = " << big_number(runs/time_diff) << " per sec avg\n"; 
}



void timing_area(){
  
  struct timeval start,end;
  std::bitset<64> dummy;
  board b;
  
  int runs = 1000000;
  
  gettimeofday(&start,NULL); 
  for(int i=0;i<runs;i++){ 
    b.reset();
    b.do_move(19,&dummy); 
  } 
  gettimeofday(&end,NULL); 
  print_diff(start,end,runs,"board::do_move()\t");
  
  
  gettimeofday(&start,NULL); 
  for(int i=0;i<runs;i++){ 
    b.reset();
    b.get_valid_moves(&dummy); 
  } 
  gettimeofday(&end,NULL); 
  print_diff(start,end,runs,"board::get_valid_moves()");
  
  runs = 100;
  gettimeofday(&start,NULL); 
  bot_ali ali(-1,8,16);
  ali.disable_shell_output();
  board ignore;
  for(int i=0;i<runs;i++){ 
    b.reset();
    ali.do_move(&b,&ignore); 
  } 
  gettimeofday(&end,NULL); 
  print_diff(start,end,runs,"bot_ali::do_move() at level 8");
  
}