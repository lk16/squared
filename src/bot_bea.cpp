#include "bot_bea.hpp"

bot_bea::bot_bea(color c, int max_depth, int max_endgame_depth):
  bot_base(c, max_depth, max_endgame_depth)
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



int bot_bea::heuristic(const board* b)
{
  int res = 0;
  
  const static int corner_factor = 10;
  const static int xsquare_factor = -2;
  const static int move_count_factor = 3;
  
  
  res += b->count_moves(c);
  res -= b->count_moves(opponent(c));
  
  res += corner_factor * std::bitset<FIELD_SIZE*FIELD_SIZE>(b->discs[c] & corners_mask).count();
  res -= corner_factor * std::bitset<FIELD_SIZE*FIELD_SIZE>(b->discs[opponent(c)] & corners_mask).count();
  
  res += xsquare_factor* std::bitset<FIELD_SIZE*FIELD_SIZE>(b->discs[c] & xsquares_mask).count();
  res -= xsquare_factor* std::bitset<FIELD_SIZE*FIELD_SIZE>(b->discs[opponent(c)] & xsquares_mask).count();
  
  
  
  return res;
}
