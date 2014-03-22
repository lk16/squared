#pragma once

#include <bitset>
#include <cassert>
#include <string.h>
#include <functional>

#include "util/const.hpp"
#include "util/math.hpp"
#include "util/macros.hpp"
#include "util/string.hpp"
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
  
  static const int location_values[10];
  
private:
  
  /// this board will be used by negamax(), negamax_exact(), heuristic(), and do_move()
  /// it represents the currently inspected move
  board inspected;
  
  int negamax_max_non_empty_fields;
  
  
public:
  bot_ali();
  bot_ali(int c, int _search_depth,int _perfect_depth);
  ~bot_ali();
  
  /// picks a move!
  virtual void do_move(const board* b,board* res);
  
  /// calculates the heuristic for this->inspected
  /// positive is good for this->turn 
  int heuristic();
  
  /** calculates the heuristic for this->inspected 
      using principal variation search
      positive result is good for this->inspected.turn
  */
  template<bool sorted> int pvs(int alpha, int beta);
  int pvs_null_window(int alpha);
  
  /// calculates the result for perfect play of this->inspected, 
  /// positive result is good for this->inspected.turn
  /// this is NOT multiplied with EXACT_SCORE_FACTOR
  int pvs_exact(int alpha, int beta);
  
  /// sort boards descending according to heurs
  void sort_boards(board *boards,int* heurs, int count);
  
  void disable_shell_output();
  
  
  
};

