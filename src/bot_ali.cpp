#include "bot_ali.hpp"

bot_ali::bot_ali(color _c, int _max_depth, int _max_endgame_depth):
  bot_base(_c, _max_depth, _max_endgame_depth)
{
}

int bot_ali::heuristic(const board* b)
{
  int res = 0;
  
  res += b->count_discs(WHITE);
  res -= b->count_discs(BLACK);
  
  
  res += b->get_mobility(WHITE);
  res -= b->get_mobility(BLACK);

  return res;
}


