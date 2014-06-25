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
#include "bots/bot_base.hpp"
#include "book/book.hpp"


#define BOT_ALI_USE_MTDF 0

class bot_ali:
  public bot_base
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
  bot_ali(int _search_depth,int _perfect_depth);
  ~bot_ali();
  
  /// picks a move!
  virtual void do_move(const board* b,board* res);
  
  virtual void disable_shell_output();
  virtual void disable_book();
  virtual void on_new_game();
  
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
  
  int id_mtdf(int depth);
  int mtdf(int f,int depth);
  
  int pvs_null_window_with_memory(int alpha);
  
  int id_mtdf_exact();
  int mtdf_exact(int f,int depth);
  
  int pvs_null_window_with_memory_exact(int alpha);
  
  
  
  /// sort boards descending according to heurs
  void sort_children(board *boards,int* heurs, int count);
  
  
  bool set_location_values_from_file(const std::string& file);
  void set_location_values(const int* v);

  const int* get_location_values() const;
  
};

