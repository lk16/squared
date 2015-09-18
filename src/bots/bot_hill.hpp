#pragma once

#include "bots/bot_base.hpp"

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
  
  // calculates the heuristic for this->inspected
  virtual int heuristic();

private:
  // HACK FIXME
  int heuristic(const board* b);
  
  // minimax because I am a lame programmer
  int minimax(const board* b,int d,bool max);
  
  int alphabeta(const board* b,int d,int alpha,int beta);
  
  
  // perform hill climbing algorithm
  int hill_climbing(board* b,int d,int pd);
};