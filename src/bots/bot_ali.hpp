#ifndef SQUARED_BOT_ALI_HPP
#define SQUARED_BOT_ALI_HPP

#include <bitset>
#include <cassert>

#include "game/util.hpp"
#include "bots/bot_base.hpp"

struct bot_ali:
  public bot_base
{
  std::bitset<TOTAL_FIELDS> location_bitsets[10];  
  
  bot_ali(color c, int max_depth, int max_endgame_depth);

  /* positive is good for WHITE */
  virtual int heuristic(const board* b);
  

  
};

#endif