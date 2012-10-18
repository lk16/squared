#include "bot_ali.hpp"

bot_ali::bot_ali(color _c, int _max_depth, int _max_endgame_depth):
  bot_base(_c, _max_depth, _max_endgame_depth)
{
}

int bot_ali::heuristic(const board* b)
{
  int disc_count_factor,mobility_factor,corner_factor,i;
  int res = 0;
  int corners[4] = {0,7,56,63};
  
  disc_count_factor = 5;
  mobility_factor = 1;
  corner_factor = 40;
  
  
  for(i=0;i<3;i++){
    if(b->discs[WHITE].to_ulong() & (1 << (corners[i]))){
      res += corner_factor;
    } 
    if(b->discs[BLACK].to_ulong() & (1 << (corners[i]))){
      res -= corner_factor;
    }
  }
  
  
  
  
  
  
  
  res += disc_count_factor * b->count_discs(WHITE);
  res -= disc_count_factor * b->count_discs(BLACK);
  
  
  res += mobility_factor * b->get_mobility(WHITE);
  res -= mobility_factor * b->get_mobility(BLACK);

  return res;
}


