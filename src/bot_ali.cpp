#include "bot_ali.hpp"

bot_ali::bot_ali(color _c, int _max_depth, int _max_endgame_depth):
  bot_base(_c, _max_depth, _max_endgame_depth)
{
}

int bot_ali::heuristic(const board* b)
{
  int res,i;
  
  const static int disc_count_factor = 2;
  const static int mobility_factor = 3;
  const static int location_value[10] = {
    30,-1,-1,-1,-4,0,1,1,1,2
  };
  
  res = 0;

  
  
  const static int location[64] = 
  {
    0,1,2,3,3,2,1,0,
    1,4,5,6,6,5,4,1,
    2,5,7,8,8,7,6,5,
    3,6,8,9,9,8,6,3,
    3,6,8,9,9,8,6,3,
    2,5,7,8,8,7,6,5,
    1,4,5,6,6,5,4,1,
    0,1,2,3,3,2,1,0
  };
  
  
  for(i=0;i<64;i++){
    if(b->discs[c].to_ulong() & (1ul << i)){
      res += location_value[location[i]];
    }
    else if(b->discs[opponent(c)].to_ulong() & (1ul << i)){
      res -= location_value[location[i]];
    } 
  }
  res += mobility_factor * b->get_mobility(c);
  res -= mobility_factor * b->get_mobility(opponent(c));

  res += disc_count_factor * b->count_discs(c);
  res -= disc_count_factor * b->count_discs(opponent(c));
  


  return res;
}


