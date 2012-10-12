#include "bot_ali.hpp"

bot_ali::bot_ali(color c, int max_depth, int max_endgame_depth):
  bot_base(c, max_depth, max_endgame_depth)
{
}

int bot_ali::heuristic(const board* b)
{
  int res = 0;
  
  res += b->count_moves(WHITE); 
  res -= b->count_moves(BLACK);

  res += b->get_mobility(WHITE);
  res -= b->get_mobility(BLACK);
  
  return res;
}


