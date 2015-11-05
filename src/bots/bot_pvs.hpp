#pragma once

#include "bots/bot_register.hpp"
#include "bots/bot_base.hpp"

class bot_pvs:
  public bot_base
{
private:
  static const int ESTIMATE_DEPTH = 4;
  static const int MIN_SORT_DEPTH = ESTIMATE_DEPTH + 3;
  static const int NO_HEURISTIC_AVAILABLE = MAX_HEURISTIC + 1;
  
  // number of recursion steps left in pvs or pvs_null_window
  int moves_left;
  
  // last heuristic
  int last_heur;
  
  // sort children
  void sort_children(board* children,board* child_end);
  
  // search for the best move
  template<bool exact>
  void search(const board* b,board* res);

  template<bool exact,bool sorted>
  int pvs(int alpha,int beta,board* b);

  template<bool exact>
  int pvs_null_window(int alpha,board* b);

  
  int look_ahead(board* b);
public:
  
  // ctor
  bot_pvs();  
  
  // dtor
  ~bot_pvs() = default;
  
  // picks a move!
  virtual void do_move(const board* b,board* res);
    
  // does something when a new game starts
  virtual void on_new_game() = 0;
  
  // calculates the heuristic for this->inspected
  virtual int heuristic(const board* b) = 0;

  // get last heuristic
  int last_heuristic() const;
 
};