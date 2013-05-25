#ifndef SQUARED_BOT_ALI_HPP
#define SQUARED_BOT_ALI_HPP

#include <bitset>
#include <cassert>

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
  
  enum{
    CORNER=0,
    NEXT_TO_CORNER=1,
    SIDE=2,
    X_SQUARE=3,
    CENTER=4  
  };
  
  std::bitset<TOTAL_FIELDS> location_bitsets[5];  
  
  bot_ali(color c, int max_depth, int max_endgame_depth);

  /// picks a move!
  virtual void do_move(const board* b,board* res);
  
  /// positive is good for WHITE 
  int heuristic(const board* b);
  
  /// calculates the heuristic for b using alpha beta pruning, positive better for white
  int alpha_beta(const board* b,int alpha, int beta,int depth_remaining);

  /// calculates the result for perfect play of board b, possitive better for white
  /// this is NOT multiplied with PERFECT_SCORE_FACTOR
  int do_move_perfect(const board* b,int alpha, int beta);
  
  /// sort boards descending according to heurs
  void sort_boards(board *boards,int* heurs, int count);
  
  /// evaluate all (count) boards at depth depth, and put the heuristics in heurs
  void evaluate_depth_level(const board* boards, int* heurs,int count,int depth);
  
};

#endif