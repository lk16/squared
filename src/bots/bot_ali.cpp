#include "bot_ali.hpp"

bot_ali::bot_ali(color _c, int _max_depth, int _max_endgame_depth):
  bot_base(_c, _max_depth, _max_endgame_depth)
{  
  int loc[TOTAL_FIELDS] = 
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
  
  
  for(int i=0;i<TOTAL_FIELDS;i++){
    location_bitsets[loc[i]].set(i);
  }
  
    
}

int bot_ali::heuristic(const board* b)
{
  /*  0,1,2,3,3,2,1,0,
      1,4,5,6,6,5,4,1,
      2,5,7,8,8,7,5,2
      3,6,8,9,9,8,6,3,
      3,6,8,9,9,8,6,3,
      2,5,7,8,8,7,5,2
      1,4,5,6,6,5,4,1,
      0,1,2,3,3,2,1,0  */
  
  
  //                              0   1   2   3   4   5   6   7   8   9
  static int open_loc_val[10] = { 50, -8, -7, -6,-10, -3, -3, -4, -3, -2 };
  static int  mid_loc_val[10] = { 40, -4, -3, -2, -6, -4, -2, -2, -2,  0 };
  static int  end_loc_val[10] = { 30,  5,  7,  9, -2,  1,  0,  3,  1,  5 };
  
  int disc_count = (b->discs[WHITE] | b->discs[BLACK]).count();
  
  int res = 0;
  
  if(disc_count<=20){
    for(int i=0;i<10;i++){
      res += open_loc_val[i] * (b->discs[WHITE] & location_bitsets[i]).count();
      res -= open_loc_val[i] * (b->discs[BLACK] & location_bitsets[i]).count();
    }
  }
  if(disc_count>=15 && disc_count<45){
    for(int i=0;i<10;i++){
      res += mid_loc_val[i] * (b->discs[WHITE] & location_bitsets[i]).count();
      res -= mid_loc_val[i] * (b->discs[BLACK] & location_bitsets[i]).count();
    }  
  }  
  if(disc_count>=40){
    for(int i=0;i<10;i++){
      res += end_loc_val[i] * (b->discs[WHITE] & location_bitsets[i]).count();
      res -= end_loc_val[i] * (b->discs[BLACK] & location_bitsets[i]).count();
    }  
  }
  
  
  return res;
}

