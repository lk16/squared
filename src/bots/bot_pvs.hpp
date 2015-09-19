#pragma once

#include "bots/bot_register.hpp"
#include "bots/bot_base.hpp"

class bot_pvs:
  public bot_base
{

  int search_max_sort_depth;
  int moves_left;
  
  static const int NORMAL_MOVE_SORT_DEPTH = 7;
  static const int PERFECT_MOVE_SORT_DEPTH = 4;
  
public:
  board inspected;
  
  // ctor
  bot_pvs() = default;  
  
  // dtor
  ~bot_pvs() = default;
  
  // picks a move!
  virtual void do_move(const board* b,board* res);
    
  // does something when a new game starts
  virtual void on_new_game() = 0;
  
  // calculates the heuristic for this->inspected
  virtual int heuristic() = 0;

  
private:
  // sort children
  void do_sorting(board* children,int n);
  
  // does the only move, no evaluation necessary
  void do_move_one_possibility(const board* b,board* res);

  // search for the best move
  template<bool exact>
  void do_move_search(const board* b,board* res);

  // performs principle variation search, unsorted
  template<bool sort,bool exact>
  int pvs(int alpha, int beta);
  
  // performs null window search
  template<bool exact>
  int pvs_null_window(int alpha);
  
  
};