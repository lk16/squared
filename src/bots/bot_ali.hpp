#pragma once

#include <bitset>
#include <cassert>
#include <string.h>
#include <functional>

#include "util/const.hpp"
#include "util/csv.hpp"
#include "util/math.hpp"
#include "util/macros.hpp"
#include "util/string.hpp"
#include "bots/bot_base.hpp"
#include "main/learn.hpp"
#include "book/book.hpp"

class bot_ali:
  public bot_base
{
  
  static const int location_values[10];
  static const int search_max_sort_depth = 6;
  
  enum eval_mode{
    ONE_MOVE_MODE,
    NORMAL_MODE,
    PERFECT_MODE,
    BOOK_MODE
  };
    
  /// this board will be used by move evaluation functions and do_move()
  /// it represents the currently inspected move
  board inspected;
  
  /// stop finite depth search when going beyond this number of discs
  int search_max_discs;
  
  book_t book;
  
    
  bool shell_output,use_book;
  
  
public:
  bot_ali(int _search_depth,int _perfect_depth);
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
  int pvs_unsorted(int alpha, int beta);
  int pvs_sorted(int alpha,int beta);
  
  int pvs_null_window(int alpha);
  
  /// calculates the result for perfect play of this->inspected, 
  /// positive result is good for this->inspected.turn
  /// this is NOT multiplied with EXACT_SCORE_FACTOR
  int pvs_exact(int alpha, int beta);
  
  /// sort boards descending according to heurs
  void sort_children(board *boards,int* heurs, int count);
  
  void disable_shell_output();
  void disable_book();
  
  
};

