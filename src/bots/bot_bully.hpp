#pragma once

#include "bots/bot_base.hpp"


struct bot_bully:
  public bot_base
{
  int moves_left;
  int search_max_sort_depth;
  
  board inspected;
  
  bot_bully();

  virtual void do_move(const board* in, board* out);

  virtual void on_new_game();

  virtual int rough_prediction(const board* b) const;

  
  void do_sorting(board* children, int child_count);
  
  // performs principle variation search, unsorted
  int pvs_unsorted(int alpha, int beta);
  
  // performs principle variation search, sorted
  int pvs_sorted(int alpha,int beta);
  
  // performs null window search
  int pvs_null_window(int alpha);
  
  // calculates the result for perfect play of this->inspected
  int pvs_exact(int alpha, int beta);
  
  // performs null window search for perfect play of this->inspected
  int pvs_exact_null_window(int alpha);
  
  int heuristic() const;
  
  void do_move_normally(const board* b, board* res);
  void do_move_perfectly(const board* b, board* res);
};