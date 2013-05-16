#include "game/board.hpp"
#include "game/util.hpp"

#define SQUARED_BOT_DISABLE_OUTPUT

#include "bots/bot_base.hpp"
#include "bots/bot_ali.hpp"

#include <ctime>
#include <iostream>
#include <algorithm>
#include <sys/time.h>

int get_descendants(board* b,int max_depth){
  
  int read_layer_size = 1;
  int write_layer_size = 0; 
  int res = 0;
  
  int read_offset = 1;
  int write_offset = read_offset + read_layer_size;
  
  for(int d=0;d<max_depth;d++){
    for(int i=0;i<read_layer_size;i++){
      int children;
      (b+read_offset+i)->get_children(b+write_offset+write_layer_size,&children);
      write_layer_size += children;
    }
    res += write_layer_size;
    read_offset = write_offset;
    read_layer_size = write_layer_size;
    write_offset += write_layer_size;
    write_layer_size = 0; 
  }
  return res;
}

void do_random_moves(board* b,int n){
  board m[32];
  int count;
  for(int i=n-1;i--;){
    b->get_children(m,&count);
    if(count == 0){
      return;
    }
    *b = m[rand()%count];
  }
}



int main(int argc,char **argv){
  int counter,i;
  timeval start,end;
  double diff;
  board in[40],out;
  int depth = fromstr<int>(std::string(argv[1]));
  bot_ali ali(WHITE,depth,depth);
  
  diff = 0.0;
  
  in[0].reset();
  for(i=1;i<40;i++){
    in[i] = in[i-1];
    do_random_moves(in+i,1);
  }
  
  gettimeofday(&start,NULL);
  for(i=1;i<40;i++){
    for(counter=0;counter<1;counter++){
      ali.do_move(in+i,&out);
    }
    std::cout << '.';
    std::cout.flush();
  }
  std::cout << '\t';
   
  gettimeofday(&end,NULL);
  diff += (end.tv_sec + (end.tv_usec / 1000000.0)) - (start.tv_sec + (start.tv_usec / 1000000.0));
  
  std::cout << "Depth " << depth << "\t\t";
  std::cout << diff << " / " << counter*i << " = " << (diff/(counter*i)) << " sec average.\n";

  return 0;
}