#ifndef SQUARED_BOT_ALI_HPP
#define SQUARED_BOT_ALI_HPP

#include <bitset>
#include <cassert>
#include <string.h>
#include <functional>
#include <unordered_map>
#include <vector>

#include "game/util.hpp"
#include "bots/bot_base.hpp"

#define BOT_ALI_ALWAYS_SHOW_HEUR         0
#define BOT_ALI_MIN_SEARCH_DEPTH_TO_SORT 5

struct bot_ali:
  public bot_base
{
  
  enum eval_mode{
    NORMAL_MODE,
    PERFECT_MODE
  };
  
  bool shell_output;
  
  static const int location_value[10];
  
  
private:
  
  // this board will be used by negamax(), negamax_exact(), heuristic(), and do_move()
  // it represents the currently inspected move
  board inspected;
  
  
  
public:

  bot_ali(int c, int _search_depth,int _perfect_depth);
  ~bot_ali();
  
  /// picks a move!
  virtual void do_move(const board* b,board* res);
  
  /// calculates the heuristic for this->inspected
  /// positive is good for this->turn 
  int heuristic();
  
  /// calculates the heuristic for this->inspected using negamax
  /// positive result is good for white
  int negamax(int alpha, int beta,int depth_remaining);
  
  /// calculates the result for perfect play of this->inspected, 
  /// positive result is good for white
  /// this is NOT multiplied with EXACT_SCORE_FACTOR
  int negamax_exact(int alpha, int beta);
  
  /// sort boards descending according to heurs
  void sort_boards(board *boards,int* heurs, int count);
  
  void disable_shell_output();
};



#endif