#ifndef SQUARED_BOT_ALI_HPP
#define SQUARED_BOT_ALI_HPP

#include <bitset>

#include "shared.hpp"
#include "bot_base.hpp"

struct bot_ali:
  public bot_base
{
  bot_ali(color c, int max_depth, int max_endgame_depth);

  /* positive is good for color this->c */
  virtual int heuristic(const board* b);
  
  std::bitset<FIELD_SIZE*FIELD_SIZE> corners_mask,xsquares_mask;
  
  
};

#endif