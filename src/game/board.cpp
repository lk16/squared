#include "board.hpp"
#include <vector>


const unsigned int board::border[64] = {
  0x2f,0x07,0x07,0x07,0x07,0x07,0x07,0x97,
  0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x94,
  0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x94,
  0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x94,
  0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x94,
  0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x94,
  0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x94,
  0xe9,0xe0,0xe0,0xe0,0xe0,0xe0,0xe0,0xf4
};

const int board::walk_diff[8][7] = {
  {- 9,-18,-27,-36,-45,-54,-63},
  {- 8,-16,-24,-32,-40,-48,-56},
  {- 7,-14,-21,-28,-35,-42,-49},
  {- 1,- 2,- 3,- 4,- 5,- 6,- 7},
  {  1,  2,  3,  4,  5,  6,  7},
  {  7, 14, 21, 28, 35, 42, 49},
  {  8, 16, 24, 32, 40, 48, 56},
  {  9, 18, 27, 36, 45, 54, 63}
};

std::bitset<64> board::bit[64];
std::bitset<64> board::location[10];

const std::bitset<64> board::walk_possible[8][7] = {
  { // up left
    std::bitset<64>(0xFEFEFEFEFEFEFEFE),
    std::bitset<64>(0xFCFCFCFCFCFCFCFC),
    std::bitset<64>(0xF8F8F8F8F8F8F8F8),
    std::bitset<64>(0xF0F0F0F0F0F0F0F0),
    std::bitset<64>(0xE0E0E0E0E0E0E0E0),
    std::bitset<64>(0xC0C0C0C0C0C0C0C0),
    std::bitset<64>(0x8080808080808080)
  },
  { // up
    std::bitset<64>(0xFFFFFFFFFFFFFF00),
    std::bitset<64>(0xFFFFFFFFFFFF0000),
    std::bitset<64>(0xFFFFFFFFFF000000),
    std::bitset<64>(0xFFFFFFFF00000000),
    std::bitset<64>(0xFFFFFF0000000000),
    std::bitset<64>(0xFFFF000000000000),
    std::bitset<64>(0xFF00000000000000)
  },
  { // up right
    std::bitset<64>(0x7F7F7F7F7F7F7F7F),
    std::bitset<64>(0x3F3F3F3F3F3F3F3F),
    std::bitset<64>(0x1F1F1F1F1F1F1F1F),
    std::bitset<64>(0x0F0F0F0F0F0F0F0F),
    std::bitset<64>(0x0707070707070707),
    std::bitset<64>(0x0303030303030303),
    std::bitset<64>(0x0101010101010101)
  },
  { // left
    std::bitset<64>(0xFEFEFEFEFEFEFEFE),
    std::bitset<64>(0xFCFCFCFCFCFCFCFC),
    std::bitset<64>(0xF8F8F8F8F8F8F8F8),
    std::bitset<64>(0xF0F0F0F0F0F0F0F0),
    std::bitset<64>(0xE0E0E0E0E0E0E0E0),
    std::bitset<64>(0xC0C0C0C0C0C0C0C0),
    std::bitset<64>(0x8080808080808080)
  },
  { // right
    std::bitset<64>(0x7F7F7F7F7F7F7F7F),
    std::bitset<64>(0x3F3F3F3F3F3F3F3F),
    std::bitset<64>(0x1F1F1F1F1F1F1F1F),
    std::bitset<64>(0x0F0F0F0F0F0F0F0F),
    std::bitset<64>(0x0707070707070707),
    std::bitset<64>(0x0303030303030303),
    std::bitset<64>(0x0101010101010101)
  },
  { // down left
  std::bitset<64>(0xFEFEFEFEFEFEFEFE),
  std::bitset<64>(0xFCFCFCFCFCFCFCFC),
  std::bitset<64>(0xF8F8F8F8F8F8F8F8),
  std::bitset<64>(0xF0F0F0F0F0F0F0F0),
  std::bitset<64>(0xE0E0E0E0E0E0E0E0),
  std::bitset<64>(0xC0C0C0C0C0C0C0C0),
  std::bitset<64>(0x8080808080808080)
  },
  { // down
    std::bitset<64>(0x00FFFFFFFFFFFFFF),
    std::bitset<64>(0x0000FFFFFFFFFFFF),
    std::bitset<64>(0x000000FFFFFFFFFF),
    std::bitset<64>(0x00000000FFFFFFFF),
    std::bitset<64>(0x0000000000FFFFFF),
    std::bitset<64>(0x000000000000FFFF),
    std::bitset<64>(0x00000000000000FF)
  },
  { // down right
  std::bitset<64>(0x7F7F7F7F7F7F7F7F),
  std::bitset<64>(0x3F3F3F3F3F3F3F3F),
  std::bitset<64>(0x1F1F1F1F1F1F1F1F),
  std::bitset<64>(0x0F0F0F0F0F0F0F0F),
  std::bitset<64>(0x0707070707070707),
  std::bitset<64>(0x0303030303030303),
  std::bitset<64>(0x0101010101010101)
  }
};


bool board::is_valid_move(int field_id) const
{ 
  std::bitset<64> moves;
  get_valid_moves(&moves);
  return (moves & board::bit[field_id]).any();
}

board* board::get_children(board* out_begin) const
{
  board* out_end = out_begin;
  std::bitset<64> valid_moves;
  std::bitset<64> to_flip,tmp_mask;
  
  
  get_valid_moves(&valid_moves);
  
  while(true){
    int field_id = find_first_set_64(valid_moves.to_ulong());
    if(field_id == -1){
      break;
    }  
    
    to_flip.reset();
  
    for(int i=0;i<8;++i){     
      
      tmp_mask.reset();
      int cur_field_id = field_id;
      
      while(true){
        
        // will i walk off the board next step?
        if((std::bitset<64>(board::border[cur_field_id]) & board::bit[i]).any()){
          break;
        }
        
        // walk ahead        
        cur_field_id += board::walk_diff[i][0]; 
        
        // current field = my color
        if((me & board::bit[cur_field_id]).any()){
          to_flip |= tmp_mask;
          break;
        }
        
        // current field = opponent color
        if((opp & board::bit[cur_field_id]).any()){
          tmp_mask.set(cur_field_id);
          continue;
        }
        
        // current fiend = empty
        break;
      }
      
    }
    
    if(out_end){
      *out_end = *this;
      out_end->me |= to_flip | board::bit[field_id];
      out_end->opp &= (~to_flip);
      out_end->switch_turn();
      ++out_end;
    }
    
    valid_moves.reset(field_id);
    
  }
  return out_end;
}

void board::get_valid_moves(std::bitset<64>* out) const
{
  out->reset();
  
  const std::bitset<64>& my_fields = this->me.to_ulong();
  const std::bitset<64>& opp_fields = this->opp.to_ulong();
  
  for(int d=4;d<8;d++){
    
    assert(board::walk_diff[d][0] > 0);
    
    *out |= 
    (
      ((opp_fields >> board::walk_diff[d][0]) & board::walk_possible[d][0]) 
      & 
      (
        ((my_fields >> board::walk_diff[d][1]) & board::walk_possible[d][1])
        |
        (
          ((opp_fields >> board::walk_diff[d][1]) & board::walk_possible[d][1])
          &
          (
            ((my_fields >> board::walk_diff[d][2]) & board::walk_possible[d][2])
            |
            (
              ((opp_fields >> board::walk_diff[d][2]) & board::walk_possible[d][2])
              &
              (
                ((my_fields >> board::walk_diff[d][3]) & board::walk_possible[d][3])
                |
                (
                  ((opp_fields >> board::walk_diff[d][3]) & board::walk_possible[d][3])
                  &
                  (
                    ((my_fields >> board::walk_diff[d][4]) & board::walk_possible[d][4])
                    |
                    (
                      ((opp_fields >> board::walk_diff[d][4]) & board::walk_possible[d][4])
                      &
                      (
                        ((my_fields >> board::walk_diff[d][5]) & board::walk_possible[d][5])
                        |
                        (
                          ((opp_fields >> board::walk_diff[d][5]) & board::walk_possible[d][5])
                          &
                          ((my_fields >> board::walk_diff[d][6]) & board::walk_possible[d][6])
                        )
                      )
                    )
                  )  
                )
              )
            )
          )
        )
      )
    );
  
  *out |= 
  (
    ((opp_fields << board::walk_diff[d][0]) & board::walk_possible[d][0]) 
    & 
    (
      ((my_fields << board::walk_diff[d][1]) & board::walk_possible[d][1])
      |
      (
        ((opp_fields << board::walk_diff[d][1]) & board::walk_possible[d][1])
        &
        (
          ((my_fields << board::walk_diff[d][2]) & board::walk_possible[d][2])
          |
          (
            ((opp_fields << board::walk_diff[d][2]) & board::walk_possible[d][2])
            &
            (
              ((my_fields << board::walk_diff[d][3]) & board::walk_possible[d][3])
              |
              (
                ((opp_fields << board::walk_diff[d][3]) & board::walk_possible[d][3])
                &
                (
                  ((my_fields << board::walk_diff[d][4]) & board::walk_possible[d][4])
                  |
                  (
                    ((opp_fields << board::walk_diff[d][4]) & board::walk_possible[d][4])
                    &
                    (
                      ((my_fields << board::walk_diff[d][5]) & board::walk_possible[d][5])
                      |
                      (
                        ((opp_fields << board::walk_diff[d][5]) & board::walk_possible[d][5])
                        &
                        ((my_fields << board::walk_diff[d][6]) & board::walk_possible[d][6])
                      )
                    )
                  )
                )  
              )
            )
          )
        )
      )
    )
  );
  }
  *out &= get_empty_fields();
}

void board::show() const
{
  int x,y;
  
  std::bitset<64> black,white;
  black = (turn==-1 ? me : opp);
  white = (turn== 1 ? me : opp);
  
  
  
  /* top line */
  std::cout << "+-----------------+\n";
  
  /* middle */
  for(y=0;y<8;y++){
    std::cout << "| ";
    for(x=0;x<8;x++){
      if(black.test(y*8+x)){
          std::cout << "\033[31;1m@\033[0m ";
      }
      else if(white.test(y*8+x)){
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

 
  
  
  count[0] = me.count();
  count[1] = opp.count();
  
  if(count[0] > count[1]){ /* I win */
    return 64 - (2*count[1]);
  }
  else if(count[1] < count[0]){ /* Opp wins */
    return -64 + (2*count[0]);
  }
  else{ /* draw */
    return 0;
  }
}

void board::do_move(int field_id, std::bitset<64>* undo_data)
{
  assert(is_valid_move(field_id));
  
  
  undo_data->reset();
  
  for(int i=0;i<8;++i){     
  
    std::bitset<64> tmp_mask;
    int cur_field_id = field_id;
    
    while(true){
      
      // will i walk off the board next step?
      if((std::bitset<64>(board::border[cur_field_id]) & board::bit[i]).any()){
        break;
      }
      
      // walk ahead        
      cur_field_id += board::walk_diff[i][0]; 
      
      // current field = my color
      if((me & board::bit[cur_field_id]).any()){
        (*undo_data) |= tmp_mask;
        break;
      }
      
      // current field = opponent color
      if((opp & board::bit[cur_field_id]).any()){
        tmp_mask |= board::bit[cur_field_id];
        continue;
      }
      
      // current fiend = empty
      break;
    }
  }
  
  
  assert((me & (*undo_data)).none());
  assert((opp & (*undo_data)) == (*undo_data));
  assert(get_non_empty_fields().test(field_id) == false);
  
  me |= ((*undo_data) | board::bit[field_id]);
  opp &= ~(*undo_data);
  
  passed = false;
  
  switch_turn();
}

void board::undo_move(int field_id, std::bitset<64>* undo_data)
{
  switch_turn();
  
  me &= ~((*undo_data));
  me &= ~(board::bit[field_id]);
  opp |= (*undo_data);  
  
  
  assert((me & (*undo_data)).none());
  assert((opp & (*undo_data)) == (*undo_data));
  assert(get_non_empty_fields().test(field_id) == false);
}


void board::init_constants()
{
  for(int i=0;i<64;i++){
    board::bit[i].reset();
    board::bit[i].set(i);
  }
  
  int locations_table[64] =
  {
    0,1,2,3,3,2,1,0,
    1,4,5,6,6,5,4,1,
    2,5,7,8,8,7,5,2,
    3,6,8,9,9,8,6,3,
    3,6,8,9,9,8,6,3,
    2,5,7,8,8,7,5,2,
    1,4,5,6,6,5,4,1,
    0,1,2,3,3,2,1,0
  };
  
  for(int i=0;i<64;i++){
    board::location[locations_table[i]].set(i);
  }
}
