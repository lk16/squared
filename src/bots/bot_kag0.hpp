/*
 * This bot is based on the java AI written by 
 * Patrick Mosca & Nathaniel Fisher
 * https://github.com/kag0/Reversi
 */

#pragma once

#include "util/bitset.hpp"
#include "util/string.hpp"
#include "bots/bot_pvs.hpp"

struct bot_kag0:
  public bot_pvs
{
  bot_kag0();  
  virtual ~bot_kag0() = default;
  
  virtual void on_new_game();
  virtual int heuristic();
  
  int get_forfeit() const;
  int get_mobility() const;
  int get_frontier() const;
  
};

