#pragma once

#include "util/bitset.hpp"
#include "util/string.hpp"
#include "bots/bot_mtdf.hpp"

struct bot_mtdf_moves:
  public bot_mtdf
{
  bot_mtdf_moves();  
  virtual ~bot_mtdf_moves();
  
  virtual void on_new_game();
  virtual int heuristic();
  
};