#ifndef SQUARED_BOT_ALI_HPP
#define SQUARED_BOT_ALI_HPP

#include <bitset>
#include <cassert>

#include "game/util.hpp"
#include "bots/bot_base.hpp"

#define bit4(a,b,c,d) (1ul<<a | 1ul<<b | 1ul<<c | 1ul<<d)
#define bit8(a,b,c,d,e,f,g,h) (bit4(a,b,c,d) | bit4(e,f,g,h))
#define bit12(a,b,c,d,e,f,g,h,i,j,k,l) (bit8(a,b,c,d,e,f,g,h) | bit4(i,j,k,l))



#define corners_mask bit4(0,7,56,63)
#define xsquares_mask bit4(9,14,49,54)


struct bot_ali:
  public bot_base
{
  bot_ali(color c, int max_depth, int max_endgame_depth);

  /* positive is good for color this->c */
  virtual int heuristic(const board* b);
  
  
  
};

#endif