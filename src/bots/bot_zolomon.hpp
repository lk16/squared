/*
 * This bot is based on the python AI written by zolomon
 * https://github.com/Zolomon/reversi-ai/blob/master/game/ai.py
 */

#pragma once

#include "util/bitset.hpp"
#include "util/string.hpp"
#include "bots/bot_pvs.hpp"

struct bot_zolomon:
  public bot_pvs
{
  bot_zolomon();  
  virtual ~bot_zolomon() = default;
  
  virtual void on_new_game();
  virtual int heuristic();
  
};

