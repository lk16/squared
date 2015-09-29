#pragma once

#include "util/bitset.hpp"
#include "util/string.hpp"
#include "bots/bot_pvs.hpp"

struct bot_moves:
  public bot_pvs
{
  bot_moves();  
  virtual ~bot_moves() = default;
  
  virtual void on_new_game();
  virtual int heuristic(const board* b);
  
};