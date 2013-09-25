#include "board.hpp"

bool board::is_valid_move(int field_id) const
{ 
  // if the square is not empty, it cannot be a valid move
  if((discs[WHITE] | discs[BLACK]).test(field_id)){
    return false;
  }
  
  // check if field_id is my color in any of the children
  board children[32];
  int move_count;
  get_children(children,&move_count);
  for(int i=0;i<move_count;i++){
    if(children[i].discs[turn].test(field_id)){
      return true;
    }
  }
  return false;
}

void board::do_move(int field_id, board* out) const
{
  *out = *this;
  
  // if the square is not empty, it cannot be a valid move 
  if((discs[WHITE] | discs[BLACK]).test(field_id)){
    CRASH;
  }
  
  
  // check if field_id is my color in any of the children
  board children[32];
  int move_count;
  get_children(children,&move_count);
  for(int i=0;i<move_count;i++){
    if(children[i].discs[turn].test(field_id)){
      *out = children[i];
      return;
    }
  }
  CRASH;
}





void board::get_children(board* out,int* move_count) const
{
  *move_count = 0;
  
  std::bitset<64> opp = discs[opponent(turn)];
  std::bitset<64> possible_moves;
  std::bitset<64> to_flip,tmp_mask;
  
  
  get_possible_moves(&possible_moves);
  
  for(int field_id=0;field_id<64;field_id++){
    
    if(!possible_moves.test(field_id)){
      continue;
    }
    to_flip.reset();
  
    for(int i=0;i<8;++i){     
      
      tmp_mask.reset();
      int cur_field_id = field_id;
      
      while(true){
        
        // will i walk off the board next step?
        if(board_border[cur_field_id] & (1ul << i)){
          break;
        }
        
        // walk ahead        
        cur_field_id += board_direction[i]; 
        
        // current field = my color
        if(discs[turn].test(cur_field_id)){
          to_flip |= tmp_mask;
          break;
        }
        
        // current field = opponent color
        if(discs[opponent(turn)].test(cur_field_id)){
          tmp_mask.set(cur_field_id);
          continue;
        }
        
        // current fiend = empty
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

void board::get_possible_moves(std::bitset<64> *out) const
{
  const std::bitset<64> *opp = &(discs[opponent(turn)]);
  
  // a field is considered a possible move when:
  // - it is horizontal/vertical/diagonal adjacent to an disc of opponent(turn)
  // - it is empty
  
  *out |= (((*opp) << 9) & std::bitset<64>(0xFEFEFEFEFEFEFEFE));
  *out |=  ((*opp) << 8);
  *out |= (((*opp) << 7) & std::bitset<64>(0x7F7F7F7F7F7F7F7F));
  *out |= (((*opp) << 1) & std::bitset<64>(0xFEFEFEFEFEFEFEFE));
  *out |= (((*opp) >> 1) & std::bitset<64>(0x7F7F7F7F7F7F7F7F));
  *out |= (((*opp) >> 7) & std::bitset<64>(0xFEFEFEFEFEFEFEFE));
  *out |=  ((*opp) >> 8);
  *out |= (((*opp) >> 9) & std::bitset<64>(0x7F7F7F7F7F7F7F7F));
  *out &= ~(discs[WHITE] | discs[BLACK]);
}




void board::show() const
{
  int x,y;
  
  /* top line */
  std::cout << "+-----------------+\n";
  
  /* middle */
  for(y=0;y<8;y++){
    std::cout << "| ";
    for(x=0;x<8;x++){
      if(discs[BLACK].test(y*8+x)){
          std::cout << "\033[31;1m@\033[0m ";
      }
      else if(discs[WHITE].test(y*8+x)){
          std::cout << "\033[34;1m@\033[0m ";
      }
      else if(is_valid_move(y*8+x)){
        std::cout << ". ";
      }  
      else{
        std::cout << "  ";
      }
    }
    std::cout << "|\n";
  }
  
  /* bottom line */
  std::cout << "+-----------------+\n";
}

int board::get_disc_diff() const
{
  int count[2];

  count[BLACK] = discs[BLACK].count();
  count[WHITE] = discs[WHITE].count();
  
  if(count[WHITE] > count[BLACK]){ /* WHITE wins */
    return 64 - (2*count[BLACK]);
  }
  else if(count[WHITE] < count[BLACK]){ /* BLACK wins */
    return -64 + (2*count[WHITE]);
  }
  else{ /* draw */
    return 0;
  }
}

bool board::try_move(int field_id, std::bitset<64>* undo_data)
{
  if((discs[BLACK] | discs[WHITE]).test(field_id)){
    return false;
  }
  
  undo_data->reset();
  
  for(int i=0;i<8;++i){     
  
    std::bitset<64> tmp_mask;
    int cur_field_id = field_id;
    
    while(true){
      
      // will i walk off the board next step?
      if(board_border[cur_field_id] & (1ul << i)){
        break;
      }
      
      // walk ahead        
      cur_field_id += board_direction[i]; 
      
      // current field = my color
      if(discs[turn].test(cur_field_id)){
        (*undo_data) |= tmp_mask;
        break;
      }
      
      // current field = opponent color
      if(discs[opponent(turn)].test(cur_field_id)){
        tmp_mask.set(cur_field_id);
        continue;
      }
      
      // current fiend = empty
      break;
    }
  }
  if(undo_data->none()){
    return false;
  }
  
  
  assert((discs[turn] & (*undo_data)) == std::bitset<64>());
  assert((discs[opponent(turn)] & (*undo_data)) == (*undo_data));
  assert((discs[WHITE] | discs[BLACK]).test(field_id) == false);
  
  discs[turn] |= ((*undo_data) | std::bitset<64>(1ul << field_id));
  discs[opponent(turn)] &= ~(*undo_data);
  
  turn = opponent(turn);
  
  return true;
}

void board::undo_move(int field_id, std::bitset<64>* undo_data)
{
  turn = opponent(turn); 
  
  discs[turn] &= ~((*undo_data));
  discs[turn].reset(field_id);
  discs[opponent(turn)] |= (*undo_data);  
  
  
  assert((discs[turn] & (*undo_data)) == std::bitset<64>());
  assert((discs[opponent(turn)] & (*undo_data)) == (*undo_data));
  assert((discs[WHITE] | discs[BLACK]).test(field_id) == false);
}

int board::get_stable_disc_count_diff(int max_depth) const
{
  board a[1000000];
  board b[1000000];
  bool expand_a = true;
  int insert_index = 0;
  int a_size = 1;
  int b_size = 0;
  a[0] = *this;
  
  for(int d=0;d<max_depth;d++){
    insert_index = 0;
    if(expand_a){
      for(int i=0;i<a_size;i++){
        int move_count;
        a[i].get_children(b+insert_index,&move_count);
        insert_index += move_count;
      }
    }
    else{
      for(int i=0;i<b_size;i++){
        int move_count;
        b[i].get_children(a+insert_index,&move_count);
        insert_index += move_count;
      }
    }
    expand_a = !expand_a;
  }
  
  board stable;
  stable.set_all();
  
  if(expand_a){
    for(int i=0;i<a_size;i++){
      stable &= a[i];
    }
  }
  else{
    for(int i=0;i<a_size;i++){
      stable &= b[i];
    }
  }
  
  return stable.discs[WHITE].count() - stable.discs[BLACK].count();
}


