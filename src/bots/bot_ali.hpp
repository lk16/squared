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
#include "util/hash_table.hpp"
#include "bots/bot_pvs.hpp"
#include "book/book.hpp"

#define BOT_ALI_USE_MTDF 0

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
    
  /// this board will be used by move evaluation functions and do_move()
  /// it represents the currently inspected move
  board inspected;
  
  /// stop finite depth search when going beyond this number of discs
  int search_max_discs;
  
  book_t book;
  
    
  bool shell_output,use_book;
  
  /// transposition table
  
  struct tpt_value{
    int lower_bound,upper_bound,best_move;
    
    tpt_value(){
      lower_bound = MIN_HEURISTIC;
      upper_bound = MAX_HEURISTIC;
      best_move = -1;
    }
    
    tpt_value(int lb,int ub,int bm){
       lower_bound = lb,upper_bound = ub,best_move = bm;
    }
  };
  
  hash_table<board,tpt_value,1000001> tpt;

  /// This strongly influences the behaviour of ali, should be constant
  /// during the entire game, but can be changed from default before the
  /// game starts with set_location_values_from_file
  int location_values[10];
  
  
public:
  
  bot_ali();  
  
  ~bot_ali();
  
  // does something when a new game starts
  virtual void on_new_game();
  
  // calculates the heuristic for this->inspected
  int heuristic();
    
  bool set_location_values_from_file(const std::string& file);
  
  void set_location_values(const int* v);

  const int* get_location_values() const;
  
};

