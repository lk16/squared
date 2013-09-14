#ifndef SQUARED_BOT_ALI_HPP
#define SQUARED_BOT_ALI_HPP

#include <bitset>
#include <cassert>
#include <string.h>

#include "game/util.hpp"
#include "bots/bot_base.hpp"

#define bit4(a,b,c,d) (1ul<<a | 1ul<<b | 1ul<<c | 1ul<<d)
#define bit8(a,b,c,d,e,f,g,h) (bit4(a,b,c,d) | bit4(e,f,g,h))
#define bit12(a,b,c,d,e,f,g,h,i,j,k,l) (bit8(a,b,c,d,e,f,g,h) | bit4(i,j,k,l))



#define corners_mask bit4(0,7,56,63)
#define xsquares_mask bit4(9,14,49,54)


struct bot_ali:
  public bot_base
{
  
  enum eval_mode{
    NORMAL_MODE,
    PERFECT_MODE
  };
  
  
  std::bitset<TOTAL_FIELDS> location_bitsets[10];  
  board board_stack[1000];
  
  bool shell_output;
  
  
  bot_ali(color c, int _search_depth, int _wld_depth,int _perfect_depth);
  ~bot_ali();
  
  /// picks a move!
  virtual void do_move(const board* b,board* res);
  
  /// positive is good for WHITE 
  int heuristic(const board* b);
  
  /// calculates the heuristic for b->turn using negamax, positive better for white
  int negamax(board* b,int alpha, int beta,int depth_remaining);
  
  /// calculates the result for perfect play of b, possitive better for white
  /// this is NOT multiplied with EXACT_SCORE_FACTOR
  int negamax_exact(board* b,int alpha, int beta);
  
  int negamax_wdl(board* b,int beta);
  
  /// sort boards descending according to heurs
  void sort_boards(board *boards,int* heurs, int count);
  
  void disable_shell_output();
};

#endif