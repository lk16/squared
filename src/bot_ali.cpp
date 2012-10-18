#include "bot_ali.hpp"

bot_ali::bot_ali(color _c, int _max_depth, int _max_endgame_depth):
  bot_base(_c, _max_depth, _max_endgame_depth)
{
}

int bot_ali::heuristic(const board* b)
{
  int disc_count_factor,mobility_factor,i;
  int res = 0;
  
  int location[64] = 
  {99,-20,-5,-5,-5,-5,-20, 99,
  -20,-40, 0, 0, 0, 0,-40,-20,
   -5,  0, 5, 5, 5, 5,  0, -5,
   -5,  0, 5,10,10, 5,  0, -5,
   -5,  0, 5,10,10, 5,  0, -5,
   -5,  0, 5, 5, 5, 5,  0, -5,
   -20,-40, 0, 0, 0, 0,-40,-20,
    99,-20,-5,-5,-5,-5,-20, 99,
  };
  
  disc_count_factor = 5;
  mobility_factor = 1;
  
  for(i=0;i<64;i++){
    if(b->discs[c].to_ulong() & (1ul << i)){
      res += location[i];
    }
    else if(b->discs[opponent(c)].to_ulong() & (1ul << i)){
      res -= location[i];
    } 
  }
  res += mobility_factor * b->get_mobility(c);
  res -= mobility_factor * b->get_mobility(opponent(c));

  res += disc_count_factor * b->count_discs(c);
  res -= disc_count_factor * b->count_discs(opponent(c));
  


  return res;
}


