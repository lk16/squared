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
  
  const static int corners_x[4] = { 0, 0, (FIELD_SIZE-1), (FIELD_SIZE-1) };
  const static int corners_y[4] = { 0, (FIELD_SIZE-1), 0, (FIELD_SIZE-1) };
  
  const static int xsquares_x[4] = { 1, 1, (FIELD_SIZE-2), (FIELD_SIZE-2) };
  const static int xsquares_y[4] = { 1, (FIELD_SIZE-2), 1, (FIELD_SIZE-2) };
  
  res = 0;
  
  for(i=0;i<4;i++){
    if(b->has_color(corners_x[i],corners_y[i],c)){
      res += corner_factor;
    }
    else if(b->has_color(corners_x[i],corners_y[i],opponent(c))){
      res -= corner_factor;
    }
    if(b->has_color(xsquares_x[i],xsquares_y[i],c)){
      res += xsquare_factor;
    }
    else if(b->has_color(xsquares_x[i],xsquares_y[i],opponent(c))){
      res -= xsquare_factor;
    }
    
  }
    
  res -= disc_count_factor * b->count_discs(c);
  res += disc_count_factor * b->count_discs(opponent(c));
  
  return res;
}


