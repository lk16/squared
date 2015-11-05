#pragma once

#include "bots/bot_base.hpp"
#include "bots/bot_moves.hpp"

struct bot_hill:
  public bot_base
{
  
 
  // ctor
  bot_hill();  
  
  // dtor
  ~bot_hill() = default;
  
  // picks a move!
  virtual void do_move(const board* b,board* res);
    
  // does something when a new game starts
  virtual void on_new_game();
  
private:
  
  // perform hill climbing algorithm
  int hill_climbing(board* b,int d,int pd);
};