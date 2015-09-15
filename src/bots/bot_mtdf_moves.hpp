#pragma once

#include "bots/bot_mtdf.hpp"
#include "util/bitset.hpp"
#include "util/string.hpp"

struct bot_mtdf_moves:
  public bot_mtdf
{
  bot_mtdf_moves();  
  ~bot_mtdf_moves() = default;
  
  virtual void on_new_game();
  virtual int heuristic();
  
};