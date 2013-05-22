#include "board.hpp"

board* board::do_move(int field_id,board* result) const
{
  static const int diff[8] = {
    -9,-8,-7,
    -1   , 1,
     7, 8, 9
  };
  
  static const int border_flag[8] = {
    0x1|0x4,0x1,0x1|0x8,
        0x4,        0x8,
    0x2|0x4,0x2,0x2|0x8
  };
  
  std::bitset<TOTAL_FIELDS> mask,tmp_mask;
  
   
  if(discs[WHITE].test(field_id) || discs[BLACK].test(field_id)){
    return result;
  }
  
  
  for(int i=0;i<8;++i){ 
    tmp_mask.reset();
    int cur_field_id = field_id;
    while(true){
      
      //test walking off the board
      if( 
        ((cur_field_id & 070)==000 && (border_flag[i] & 0x1)) ||
        ((cur_field_id & 070)==070 && (border_flag[i] & 0x2)) ||
        ((cur_field_id & 007)==000 && (border_flag[i] & 0x4)) ||
        ((cur_field_id & 007)==007 && (border_flag[i] & 0x8)) ||
        false
      ){
        break;
      }
      cur_field_id += diff[i]; 
            
      
      if(discs[opponent(turn)].test(cur_field_id)){
        tmp_mask.set(cur_field_id);
        continue;
      }
      
      if(discs[turn].test(cur_field_id)){
        if(tmp_mask.any()){
          mask |= tmp_mask;
        }
        break;
      }
      
      // cur_field_id == EMPTY
      break;
      
    }
  }
  
  if(mask.any()){
    *result = *this;
    result->discs[turn] |= mask;
    result->discs[turn].set(field_id);
    result->discs[opponent(turn)] &= (~mask);
    result->turn = opponent(result->turn);
    return ++result;
  }
  return result;
}

void board::get_children(board* array,int* move_count) const
{
  board* cur = array;
  for(int field_id=0;field_id<TOTAL_FIELDS;field_id++){
    cur = do_move(field_id,cur);
  }
  *move_count = (cur-array);
}

bool board::has_moves() const
{
  board dummy;
  for(int field_id=0;field_id<TOTAL_FIELDS;field_id++){
    if(&dummy != do_move(field_id,&dummy)){
      return true;
    }
  }
  return false;
}


void board::show() const
{
  int x,y;
  board dummy;
  
  /* top line */
  std::cout << "+-";
  for(x=0;x<FIELD_SIZE;x++){
    std::cout << "--";
  }
  std::cout << "+\n";
  
  /* middle */
  for(y=0;y<FIELD_SIZE;y++){
    std::cout << "| ";
    for(x=0;x<FIELD_SIZE;x++){
      if(discs[BLACK].test(y*FIELD_SIZE+x)){
          std::cout << "\033[31;1m@\033[0m";
      }
      else if(discs[WHITE].test(y*FIELD_SIZE+x)){
          std::cout << "\033[34;1m@\033[0m";
      }
      else if(&dummy != do_move(y*FIELD_SIZE+x,&dummy)){
        std::cout << ". ";
      }  
      else{
        std::cout << "  ";
      }
    }
    std::cout << "|\n";
  }
  
  /* bottom line */
  std::cout << "+-";
  for(x=0;x<FIELD_SIZE;x++){
    std::cout << "--";
  }
  std::cout << "+\n";
}

int board::get_disc_diff() const
{
  int count[2];

  count[BLACK] = discs[BLACK].count();
  count[WHITE] = discs[WHITE].count();
  
  if(count[WHITE] > count[BLACK]){ /* c wins */
    return (TOTAL_FIELDS)-(2*count[BLACK]);
  }
  else if(count[WHITE] < count[BLACK]){ /* c loses */
    return ((-TOTAL_FIELDS)+(2*count[WHITE]));
  }
  else{ /* draw */
    return 0;
  }
}
