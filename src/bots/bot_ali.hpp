#pragma once

#include <bitset>
#include <cassert>
#include <string.h>
#include <functional>
#include <string>

#include "util/const.hpp"
#include "util/csv.hpp"
#include "util/math.hpp"
#include "util/macros.hpp"
#include "util/string.hpp"
#include "bots/bot_pvs.hpp"
#include "book/book.hpp"

class bot_ali:
  public bot_pvs
{
  friend class bot_ali_trainer;
  
  static const int search_max_sort_depth = 6;
  
  enum eval_mode{
    ONE_MOVE_MODE,
    NORMAL_MODE,
    PERFECT_MODE,
    BOOK_MODE
  };
    
  // these values pretty much define the behaviour of bot_ali
  static const int location_values[10];
  
  /// this board will be used by move evaluation functions and do_move()
  /// it represents the currently inspected move
  board inspected;
  
  /// stop finite depth search when going beyond this number of discs
  int search_max_discs;
  
  book_t book;
  
    
  bool shell_output,use_book;
  
  /// transposition table
  
   

  
  
public:
  
  bot_ali();  
  
  ~bot_ali();
  
  // does something when a new game starts
  virtual void on_new_game();
  
  // calculates the heuristic for this->inspected
  int heuristic();

  
};

