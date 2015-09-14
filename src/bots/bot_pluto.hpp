#pragma once

#include "bots/bot_mtdf.hpp"
#include "util/bitset.hpp"
#include "util/string.hpp"

struct bot_pluto:
  public bot_mtdf
{
  bot_pluto();  
  ~bot_pluto() = default;
  
  virtual void on_new_game();
  virtual int heuristic();
  
};