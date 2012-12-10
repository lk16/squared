#include "bot_ali.hpp"

bot_ali::bot_ali(color _c, int _max_depth, int _max_endgame_depth):
  bot_base(_c, _max_depth, _max_endgame_depth)
{
}

int bot_ali::heuristic(const board* b)
{
  int res,i;
  const static int disc_count_factor = 2;
  const static int corner_factor = 30;
  const static int xsquare_factor = -4;
  
  const static int corners[4] = {
    0 + FIELD_SIZE * 0,
    0 + FIELD_SIZE * (FIELD_SIZE-1),
    (FIELD_SIZE-1) + FIELD_SIZE * 0,
    (FIELD_SIZE-1) + FIELD_SIZE * (FIELD_SIZE-1)
  };
  
  const static int xsquares[4] = {
    1 + FIELD_SIZE * 1,
    1 + FIELD_SIZE * (FIELD_SIZE-2),
    (FIELD_SIZE-2) + FIELD_SIZE * 0,
    (FIELD_SIZE-2) + FIELD_SIZE * (FIELD_SIZE-2)
  };
  
  res = 0;
  
  for(i=0;i<4;i++){
    if(b->discs[c].to_ulong() & (1ul << (corners[i]))){
      res += corner_factor;
    }
    else if(b->discs[opponent(c)].to_ulong() & (1ul << (corners[i]))){
      res -= corner_factor;
    }
    if(b->discs[c].to_ulong() & (1ul << (xsquares[i]))){
      res += xsquare_factor;
    }
    else if(b->discs[opponent(c)].to_ulong() & (1ul << (xsquares[i]))){
      res -= xsquare_factor;
    }
    
  }
    
  res -= disc_count_factor * b->count_discs(c);
  res += disc_count_factor * b->count_discs(opponent(c));
  
  return res;
}


