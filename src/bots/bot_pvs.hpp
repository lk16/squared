#pragma once

#include "bots/bot_register.hpp"
#include "bots/bot_base.hpp"

class bot_pvs:
  public bot_base
{

  int search_max_sort_depth;
  int moves_left;
  
public:
  
  // ctor
  bot_pvs() = default;  
  
  // dtor
  ~bot_pvs() = default;
  
  // picks a move!
  virtual void do_move(const board* b,board* res);
    
  // does something when a new game starts
  virtual void on_new_game() = 0;
  
  // calculates the heuristic for this->inspected
  virtual int heuristic(const board* b) = 0;

  
private:
  int LOOK_AHEAD_DEPTH = 4;
  
  // search for the best move
  template<bool exact>
  void search(const board* b,board* res);

  template<bool exact>
  int pvs(int alpha,int beta,const board* b);

  template<bool exact>
  int pvs_null_window(int alpha,const board* b);

  
  int look_ahead(const board* b);
};