#include "testing.hpp"
#include "bots/bot_ali.hpp"
#include "util/csv.hpp"

#include <future>
#include <vector>
#include <set>
#include <unistd.h>

void print_time_diff(timeval start,timeval end,int runs,const std::string& name){
  
  double time_diff = (end.tv_sec + (end.tv_usec / 1000000.0)) -
  (start.tv_sec + (start.tv_usec / 1000000.0)); 
  std::cout << name << '\t' << runs << " / " << time_diff; 
  std::cout << " = " << big_number(runs/time_diff) << " per sec avg\n"; 
}


std::set<board> generate_all_boards_at_depth(int depth,board start){
 
  std::set<board> tmp_set,res;
  board* tmp_array = new board[32];
  board* tmp_array_end;
  
  res.insert(start);
  
  for(int d=0;d<depth;d++){
    for(board it: res){
      tmp_array_end = it.get_children(tmp_array);
      tmp_set.insert(tmp_array,tmp_array_end);
    }
    res.swap(tmp_set);
    tmp_set.clear();
  }
  
  delete[] tmp_array;
  return res;
}



void testing_area(){
  
  std::cout << (1ull << 0) << '\n';
  
  
#if 0  
  const int diff[8] = {-9,-8,-7,-1,1,7,8,9};
  
  board b1,b2,original;
  bits64 r1,r2;
  
  
  for(int i=0;i<64;i++){
    for(int d=0;d<8;d++){
      for(int dist=1;dist<6;dist++){
        b1.reset();
        b1.opp = 0x0;
        b1.me = (1ull << i);

        int tmp = i + diff[d]*dist;
        if(tmp>=0 && tmp<=63){
          b1.opp |= (1ull << tmp);
        }
        int move = i+(diff[d]*(dist+1));
        if(move>=0 && move<=63){
          b2 = original = b1;
          r1 = b1.do_move(move);
          //r2 = b2.do_move_experimental(move);
          r2 = (b2.*move_funcs[move])();
          if(r1!=r2 || !(b1==b2)){
            original.show();
            b1.show();
            b2.show();
            bits_show(r1);
            bits_show(r2);
            return;
          }
        }
      }
    }
  }
#endif 

  //board b,c;
  
  /*b.reset();
  c.reset();
  b.do_move(19);
  c.do_move_field_id(19);
  b.show();
  c.show();
  
  b.reset();
  c.reset();
  b.do_move(44);
  c.do_move_field_id(44);
  b.show();
  c.show();*/
  
  /*b.reset();
  c.reset();
  b.do_move(37);
  c.do_move_field_id(37);
  b.show();
  c.show();
  
  b1.reset();
  c.reset();
  b1.do_move(26);
  c.do_move_experimental(26);
  b1.show();
  c.show();
  
  b1.do_move(18);
  c.do_move_experimental(18);
  b1.show();
  c.show();*/
  
  
  
#if 0
  for(bits i=0;i<65536;i++){
    std::cout << count_64(i) << ',';
    if(i%32 == 31){
      std::cout << '\n';
    }
  }
#endif

#if 0
  board b;
  b.reset();  
  
  timeval start,end;
  
  bot_ali bot(11,11);
  bot.disable_shell_output();
  board tmp;
  
  
  gettimeofday(&start,NULL);
  
  gettimeofday(&end,NULL);
  print_time_diff(start,end,1,"evaluating boards");
  
#endif  
  
#if 0
  
  
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
#endif  
}
