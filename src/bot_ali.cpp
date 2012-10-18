#include "bot_ali.hpp"

bot_ali::bot_ali(color _c, int _max_depth, int _max_endgame_depth):
  bot_base(_c, _max_depth, _max_endgame_depth)
{
}

int bot_ali::heuristic(const board* b)
{
  int disc_count_factor,mobility_factor,corner_factor,xsquare_factor,i;
  int res = 0;
  int corners[4] = {0,7,56,63};
  int xsquares[4] = {9,14,49,54};
  
  disc_count_factor = 5;
  mobility_factor = 1;
  corner_factor = 100;
  xsquare_factor = -40;
  
  for(i=0;i<3;i++){
    if(b->has_color(corners[i]%8,corners[i]/8,WHITE)){
      res += corner_factor;
    } 
    if(b->has_color(corners[i]%8,corners[i]/8,BLACK)){
      res -= corner_factor;
    }
    if(b->has_color(xsquares[i]%8,xsquares[i]/8,WHITE)){
      res += xsquare_factor;
    }
    if(b->has_color(xsquares[i]%8,xsquares[i]/8,BLACK)){
      res -= xsquare_factor;
    }
  }
  res += mobility_factor * b->get_mobility(WHITE);
  res -= mobility_factor * b->get_mobility(BLACK);

/*  
  res += disc_count_factor * b->count_discs(WHITE);
  res -= disc_count_factor * b->count_discs(BLACK);
*/  
  


  return res;
}


