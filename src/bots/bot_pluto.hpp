#pragma once

#include "util/bitset.hpp"
#include "util/string.hpp"
#include "bots/bot_mtdf.hpp"

struct bot_pluto:
  public bot_mtdf
{
  bot_pluto();  
  ~bot_pluto() = default;
  
  virtual void on_new_game();
  virtual int heuristic();
  
};