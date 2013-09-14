#include "board.hpp"

board* board::do_move(int field_id,board* result) const
{ 
  std::bitset<TOTAL_FIELDS> mask,tmp_mask;
  
   
  if(discs[WHITE].test(field_id) || discs[BLACK].test(field_id)){
    return result;
  }
  
  
  for(int i=0;i<8;++i){ 
    tmp_mask.reset();
    int cur_field_id = field_id;
    while(true){
      
      //test walking off the board
      if(board_border[cur_field_id] & (1ul << i)){
        break;
      }
      
      cur_field_id += board_direction[i]; 
      
      if(discs[opponent(turn)].test(cur_field_id)){
        tmp_mask.set(cur_field_id);
        continue;
      }
      
      if(discs[turn].test(cur_field_id)){
        mask |= tmp_mask;
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

void board::get_children(board* out,int* move_count) const
{
  *move_count = 0;
  
  std::bitset<64> opp = discs[opponent(turn)];
  std::bitset<64> possible_moves;
  std::bitset<64> to_flip,tmp_mask;
  
  
  // a field is considered a possible move when:
  // - it is horizontal/vertical/diagonal adjacent to an disc of opponent(turn)
  // - it is empty
  possible_moves |= ((opp << 9) & std::bitset<64>(0xFEFEFEFEFEFEFEFE));
  possible_moves |=  (opp << 8);
  possible_moves |= ((opp << 7) & std::bitset<64>(0x7F7F7F7F7F7F7F7F));
  possible_moves |= ((opp << 1) & std::bitset<64>(0xFEFEFEFEFEFEFEFE));
  possible_moves |= ((opp >> 1) & std::bitset<64>(0x7F7F7F7F7F7F7F7F));
  possible_moves |= ((opp >> 7) & std::bitset<64>(0xFEFEFEFEFEFEFEFE));
  possible_moves |=  (opp >> 8);
  possible_moves |= ((opp >> 9) & std::bitset<64>(0x7F7F7F7F7F7F7F7F));
  possible_moves &= ~(discs[WHITE] | discs[BLACK]);
  
  for(int field_id=0;field_id<64;field_id++){
    
    if(!possible_moves.test(field_id)){
      continue;
    }
    to_flip.reset();
  
    for(int i=0;i<8;++i){     
      
      tmp_mask.reset();
      int cur_field_id = field_id;
      while(true){
        
        //test walking off the board
        if(board_border[cur_field_id] & (1ul << i)){
          break;
        }
        
        cur_field_id += board_direction[i]; 
        if(discs[turn].test(cur_field_id)){
          to_flip |= tmp_mask;
          break;
        }
        if(discs[opponent(turn)].test(cur_field_id)){
          tmp_mask.set(cur_field_id);
          continue;
        }
        
        
        
        
        // cur_field_id == EMPTY
        break;
      }
    }
    
    if(to_flip.any()){
      if(out){
        memcpy(out,this,sizeof(board));
        out->discs[turn] |= to_flip | std::bitset<64>(1ul << field_id);
        out->discs[opponent(turn)] &= (~to_flip);
        out->turn = opponent(out->turn);
        ++out;
      }
      ++(*move_count);
    }  
  }
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
          std::cout << "\033[31;1m@\033[0m ";
      }
      else if(discs[WHITE].test(y*FIELD_SIZE+x)){
          std::cout << "\033[34;1m@\033[0m ";
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
  
  if(count[WHITE] > count[BLACK]){ /* WHITE wins */
    return (TOTAL_FIELDS)-(2*count[BLACK]);
  }
  else if(count[WHITE] < count[BLACK]){ /* BLACK wins */
    return ((-TOTAL_FIELDS)+(2*count[WHITE]));
  }
  else{ /* draw */
    return 0;
  }
}