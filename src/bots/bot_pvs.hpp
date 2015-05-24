#pragma once

#include "bots/bot_register.hpp"
#include "bots/bot_base.hpp"

class bot_pvs:
  public bot_base
{
  
  enum eval_mode{
    ONE_MOVE_MODE,
    NORMAL_MODE,
    PERFECT_MODE,
    BOOK_MODE
  };
  
  int search_max_sort_depth;
  int moves_left;
  
  static const int NORMAL_MOVE_SORT_DEPTH = 8;
  static const int PERFECT_MOVE_SORT_DEPTH = 5;
  
public:
  board inspected;
  
  bot_pvs();  
  
  ~bot_pvs();
  
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

  // evaluates and performs a move normally
  void do_move_normally(const board* b,board* res);
  
  // evaluates and performs the perfect move
  void do_move_perfectly(const board* b,board* res);
  
  // performs best move if found in book, returns whether successful
  bool do_move_book(const board* b,board* res);

  // performs principle variation search, unsorted
  template<bool sorted,bool exact>
  int pvs(int alpha, int beta);
  
  // performs null window search
  int pvs_null_window(int alpha);
  
  // performs null window search for perfect play of this->inspected
  int pvs_exact_null_window(int alpha);
  
};