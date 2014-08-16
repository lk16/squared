#pragma once

#include "util/bitset.hpp"
#include "util/string.hpp"
#include "bots/bot_base.hpp"

struct bot_moves:
  public bot_base
{
  bot_moves();  
  
  virtual void do_move(const board* in, board* out);
  virtual void on_new_game();
  virtual ~bot_moves();
  
  int alpha_beta(const board* b,int alpha,int beta);
  int heuristic(const board* b);
  
  int search_max_discs;
};