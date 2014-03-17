#include "testing.hpp"
#include "bots/bot_ali.hpp"

#include <future>
#include <vector>

void print_time_diff(timeval start,timeval end,int runs,const char* name){
  
  double time_diff = (end.tv_sec + (end.tv_usec / 1000000.0)) -
  (start.tv_sec + (start.tv_usec / 1000000.0)); 
  std::cout << name << '\t' << runs << " / " << time_diff; 
  std::cout << " = " << big_number(runs/time_diff) << " per sec avg\n"; 
}


void testing_area(){
  
  timeval start,end;
  
  board b1,b2;
  b1.reset();
  b2.reset();
  bot_ali ali(BLACK,8,8);
  bot_ali ali2(BLACK,8,8);
  board out,out2;
  
  gettimeofday(&start,NULL);
  ali.do_move(&b1,&out);
  gettimeofday(&end,NULL);
 
  print_time_diff(start,end,1,"ali.do_move() sequential");
  
  
  /*std::future<void> f1,f2;
  
  gettimeofday(&start,NULL);
  f1 = std::async(std::launch::async,&bot_ali::do_move,&ali,&b1,&out);
  f2 = std::async(std::launch::async,&bot_ali::do_move,&ali2,&b2,&out2);
  f1.get();
  f2.get();
  gettimeofday(&end,NULL); 
  */
  
  int n = 2;
  
  std::vector<std::future<void> > fut;
  std::vector<bot_ali> alis;
  std::vector<board> boards,outs;
  
  gettimeofday(&start,NULL);
  for(int i=0;i<n;i++){
    alis.push_back(bot_ali(BLACK,8,8));    
    boards.push_back(board());
    boards.back().reset();
    outs.push_back(board());
  }
  for(int i=0;i<n;i++){
    fut.push_back(
      std::async(&bot_ali::do_move,&alis[i],&boards[i],&outs[i])
    );
  }
  for(int i=0;i<n;i++){
    fut[i].get();
  }
  gettimeofday(&end,NULL);
  
  
  print_time_diff(start,end,n,"ali.do_move() parallel");
  
}
