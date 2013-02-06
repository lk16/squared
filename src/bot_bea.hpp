#ifndef SQUARED_BOT_BEA_HPP
#define SQUARED_BOT_BEA_HPP

#include <bitset>

#include "shared.hpp"
#include "bot_base.hpp"

struct bot_bea:
public bot_base
{
  bot_bea(color c, int max_depth, int max_endgame_depth);
  
  /* positive is good for color this->c */
  virtual int heuristic(const board* b);
  
  std::bitset<FIELD_SIZE*FIELD_SIZE> corners_mask,xsquares_mask;
  
};

#endif