#include "testing.hpp"
#include "bots/bot_ali.hpp"
#include "util/csv.hpp"
#include "util/hash_table.hpp"

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

board from_move_sequence(std::string in){
  board b;
  b.reset();
  for(int i=0;i<(int)in.size()/2;i++){
    int move = (in[2*i] - 'a') + (8 * (in[(2*i)+1] - '1'));
    b.do_move(move);
  }
  return b;
}


void testing_area(){
  for(bits64 i=0;i<256;i++){
    std::cout << std::showbase << std::hex << rotate_left(i) << '\n';
  }
  
  
}
