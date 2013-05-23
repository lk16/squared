#include "bot_ali.hpp"

bot_ali::bot_ali(color _c, int _max_depth, int _max_endgame_depth):
  bot_base(_c, _max_depth, _max_endgame_depth)
{
}

int bot_ali::heuristic(const board* b)
{
  int res;
  
  /*
  disc  corner  xsquare
  -1    20      -4       << working
  */
  
  
  int disc_count_factor,corner_factor,xsquare_factor;
  
  if(std::bitset<TOTAL_FIELDS>(b->discs[WHITE] | b->discs[BLACK]).count() <= 48){
    disc_count_factor = -1;
    corner_factor = 20;
    xsquare_factor = -4;
  }
  else{
    disc_count_factor = 1;
    corner_factor = 20;
    xsquare_factor = 0;
  }
  
  
  
  
  res = 0;
  
  res += corner_factor * std::bitset<TOTAL_FIELDS>(
  b->discs[c].to_ulong() & corners_mask).count();
  res -= corner_factor * std::bitset<TOTAL_FIELDS>(
  b->discs[opponent(c)].to_ulong() & corners_mask).count();
  res += xsquare_factor* std::bitset<TOTAL_FIELDS>(
  b->discs[c].to_ulong() & xsquares_mask).count();
  res -= xsquare_factor* std::bitset<TOTAL_FIELDS>(
  b->discs[opponent(c)].to_ulong() & xsquares_mask).count();
  
  res += disc_count_factor * b->count_discs(c);
  res -= disc_count_factor * b->count_discs(opponent(c));
  
  return res;
}

