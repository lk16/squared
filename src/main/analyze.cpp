#include "game/util.hpp"
#include "game/board.hpp"
#include "bots/bot_ali.hpp"

#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <vector>

/**
  @param in an input board
  @param out the best move according to the heuristic of bot_ali, or NULL if no moves
  @return whether a move could be done
*/  
bool do_good_move(const board* in,board* out){
  board moves[1000];
  int move_count;
  moves[0] = *in;
  
  moves[0].get_children(moves+1,&move_count);
  if(move_count==0){
    moves[0].turn = opponent(moves[0].turn);
    moves[0].get_children(moves+1,&move_count);
    if(move_count==0){
      out = NULL;
      return false;
    }
  }
  const int ali_level = 3;
  bot_ali ali(out->turn,ali_level,ali_level,ali_level);
  ali.disable_shell_output();
  ali.do_move(moves,out);
  return true;
}


/**
  @param in an input board
  @param out if a random move was possible a copy of @p in after random move, NULL otherwise
  @return whether a move could be done
*/
bool do_random_move(const board* in,board* out){
  
  board moves[32];
  int move_count;
  *out = *in;
  
  out->get_children(moves+1,&move_count);
  if(move_count==0){
    out->turn = opponent(out->turn);
    out->get_children(moves+1,&move_count);
    if(move_count==0){
      out = NULL;
      return false;
    }
  }
  *out = moves[1 + (rand() % move_count)];
  return true;
}

bool do_good_moves(const board* in,board* out,int disc_count){
  *out = *in;
  for(int i=0;i<(disc_count-4);i++){
    if(!do_good_move(out,out)){
      return false;
    }
  }
  return true;
}

/**
  @param in input board
  @param out in after doing a number of random moves
  @param disc_count the number of discs in out if successful
  @return whether successful
*/
bool do_random_moves(const board* in,board* out,int disc_count){
  *out = *in;
  for(int i=0;i<(disc_count-4);i++){
    if(!do_random_move(out,out)){
      return false;
    }
  }
  return true;
}

/**
  @param b input board
*/
int count_discs(const board* b){
  return std::bitset<64>(b->discs[WHITE] | b->discs[BLACK]).count();
}

/**
  @param out the reasonable board
  @param num_discs number of discs expected on @p out
*/
void get_reasonable_board(board* out,int num_discs){
  
  // in procents
  const int chance_random_move = 5;
  
  out->reset();
  assert(out->turn == WHITE || out->turn == BLACK);
  
  while(count_discs(out)!=num_discs){
    if(rand() % 100 < chance_random_move){
      if(!do_random_move(out,out)){
        out->reset();
      }
    }
    else{
      if(!do_good_move(out,out)){
        out->reset();
      }
    }
  }
}

void print_stats(const board* b){
  int stats_size = 40;
  int stats[stats_size];
  for(int i=0;i<stats_size;i++){
    stats[i] = 0;
  }
  
  int pos[64] = {
    0,1,2,3,3,2,1,0,
    1,4,5,6,6,5,4,1,
    2,5,7,8,8,7,5,2,
    3,6,8,9,9,8,6,3,
    3,6,8,9,9,8,6,3,
    2,5,7,8,8,7,5,2,
    1,4,5,6,6,5,4,1,
    0,1,2,3,3,2,1,0
  };
  std::bitset<64> pos_bs[10];
  for(int i=0;i<64;i++){
    pos_bs[pos[i]].set(i);
  }
  
  board stack[1000];
  stack[0] = *b;
  
  //stats[0] = -bot_ali(WHITE,60,60,60).negamax_exact(stack,-64,64);
  CRASH;
  
  for(int i=0;i<10;i++){
    stats[ 1+i] = std::bitset<64>(b->discs[BLACK] & pos_bs[i]).count();
    stats[11+i] = std::bitset<64>(b->discs[WHITE] & pos_bs[i]).count();
  }

  
  
  for(color c=BLACK;c<=WHITE;c=(color)(c+1)){
    int opp = opponent(c);
    stack[0].turn = c;
    stack[0].get_children(stack+1,stats+(c==BLACK ? 21 : 22));
    std::bitset<64> stable = b->discs[opp];
    for(int i=0;i<stats[c==BLACK ? 21 : 22];i++){
      stable &= stack[1+i].discs[opp];
      stats[c==BLACK ? 25 : 26] += ((~stack[0].discs[c]) & stack[1+i].discs[c]).count();
    }
    stats[c==BLACK ? 23 : 24] = stable.count();
  }



  
  // output the collected data
  for(int i=0;i<stats_size;i++){
    std::cout << stats[i] << ((i==stats_size-1) ? '\n' : ',');
  }
}

long get_time_diff(timeval* start,timeval* end){
  return 1000000l * (end->tv_sec - start->tv_sec)
  + (end->tv_usec - start->tv_usec);
}


int main(){
#if 1
  time_t seed = time(NULL);
  srand(seed);
  std::cout << "Seed: " << seed << "\n";
#endif

  board x;
  
  for(int count=1;true;count++){
    get_reasonable_board(&x,48);
    print_stats(&x);
  }
  return 0;
}
