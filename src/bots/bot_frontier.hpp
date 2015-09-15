#pragma once

#include "util/bitset.hpp"
#include "util/string.hpp"
#include "bots/bot_pvs.hpp"

struct bot_frontier:
  public bot_pvs
{
  bot_frontier();  
  virtual ~bot_frontier();
  
  virtual void on_new_game();
  virtual int heuristic();
  
};