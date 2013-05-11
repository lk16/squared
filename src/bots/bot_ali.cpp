#include "bot_ali.hpp"

bot_ali::bot_ali(color _c, int _max_depth, int _max_endgame_depth):
  bot_base(_c, _max_depth, _max_endgame_depth)
{
  corners_mask.set(0 + FIELD_SIZE * 0);
  corners_mask.set(0 + FIELD_SIZE * (FIELD_SIZE-1));
  corners_mask.set((FIELD_SIZE-1) + FIELD_SIZE * 0);
  corners_mask.set((FIELD_SIZE-1) + FIELD_SIZE * (FIELD_SIZE-1));

  xsquares_mask.set(1 + FIELD_SIZE * 1);
  xsquares_mask.set(FIELD_SIZE*2 - 2);
  xsquares_mask.set( (FIELD_SIZE*FIELD_SIZE) - (2 + FIELD_SIZE * 1) );
  xsquares_mask.set( (FIELD_SIZE*FIELD_SIZE) - (FIELD_SIZE*2 - 1) );
  
}

int bot_ali::heuristic(const board* b)
{
  int res;
  
  /*
  disc  corner  xsquare
  -1    20      -4       << working
  */
  
  const static int disc_count_factor = -1;
  const static int corner_factor = 20;
  const static int xsquare_factor = -4;
  
  res = 0;
  
  res += corner_factor * std::bitset<FIELD_SIZE*FIELD_SIZE>(b->discs[c] & corners_mask).count();
  res -= corner_factor * std::bitset<FIELD_SIZE*FIELD_SIZE>(b->discs[opponent(c)] & corners_mask).count();
  res += xsquare_factor* std::bitset<FIELD_SIZE*FIELD_SIZE>(b->discs[c] & xsquares_mask).count();
  res -= xsquare_factor* std::bitset<FIELD_SIZE*FIELD_SIZE>(b->discs[opponent(c)] & xsquares_mask).count();
  
  res += disc_count_factor * b->count_discs(c);
  res -= disc_count_factor * b->count_discs(opponent(c));
  
  return res;
}

