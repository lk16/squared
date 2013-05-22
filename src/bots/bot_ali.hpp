#ifndef SQUARED_BOT_ALI_HPP
#define SQUARED_BOT_ALI_HPP

#include <bitset>
#include <cassert>

#include "game/util.hpp"
#include "bots/bot_base.hpp"

struct bot_ali:
  public bot_base
{
  std::bitset<TOTAL_FIELDS> location_bitsets[10];  
  
  bot_ali(color c, int max_depth, int max_endgame_depth);

  /// picks a move!
  virtual void do_move(const board* b,board* res);
  
  /// positive is good for WHITE 
  int heuristic(const board* b);
  
  /// calculates the heuristic for b using alpha beta pruning
  int alpha_beta(const board* b,int alpha, int beta,int depth_remaining);
  
  /// sort boards descending according to heurs
  void sort_boards(board *boards,int* heurs, int count);
  
  /// evaluate all (count) boards at depth depth, and put the heuristics in heurs
  void evaluate_depth_level(const board* boards, int* heurs,int count,int depth);
  
};

#endif