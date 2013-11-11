#include "board.hpp"

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
  {- 9,-18,-27,-36,-45,-54,-63}, // 0: up left
  {- 8,-16,-24,-32,-40,-48,-56}, // 1: up 
  {- 7,-14,-21,-28,-35,-42,-49}, // 2: up right
  {- 1,- 2,- 3,- 4,- 5,- 6,- 7}, // 3: left
  {  1,  2,  3,  4,  5,  6,  7}, // 4: right
  {  7, 14, 21, 28, 35, 42, 49}, // 5: down left
  {  8, 16, 24, 32, 40, 48, 56}, // 6: down
  {  9, 18, 27, 36, 45, 54, 63}  // 7: down right
};

const std::bitset<64> board::walk_possible[8][7] = {
  { // up left
    std::bitset<64>(0xFEFEFEFEFEFEFE00),
    std::bitset<64>(0xFCFCFCFCFCFC0000),
    std::bitset<64>(0xF8F8F8F8F8000000),
    std::bitset<64>(0xF0F0F0F000000000),
    std::bitset<64>(0xE0E0E00000000000),
    std::bitset<64>(0xC0C0000000000000),
    std::bitset<64>(0x8000000000000000)
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
    std::bitset<64>(0x7F7F7F7F7F7F7F00),
    std::bitset<64>(0x3F3F3F3F3F3F0000),
    std::bitset<64>(0x1F1F1F1F1F000000),
    std::bitset<64>(0x0F0F0F0F00000000),
    std::bitset<64>(0x0707070000000000),
    std::bitset<64>(0x0303000000000000),
    std::bitset<64>(0x0100000000000000)
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
  std::bitset<64>(0x00FEFEFEFEFEFEFE),
  std::bitset<64>(0x0000FCFCFCFCFCFC),
  std::bitset<64>(0x000000F8F8F8F8F8),
  std::bitset<64>(0x00000000F0F0F0F0),
  std::bitset<64>(0x0000000000E0E0E0),
  std::bitset<64>(0x000000000000C0C0),
  std::bitset<64>(0x0000000000000080)
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
  std::bitset<64>(0x007F7F7F7F7F7F7F),
  std::bitset<64>(0x00003F3F3F3F3F3F),
  std::bitset<64>(0x0000001F1F1F1F1F),
  std::bitset<64>(0x000000000F0F0F0F),
  std::bitset<64>(0x0000000000070707),
  std::bitset<64>(0x0000000000000303),
  std::bitset<64>(0x0000000000000001)
  }
};

const std::bitset<64> board::bit[64] = {
  (1ul <<  0),(1ul <<  1),(1ul <<  2),(1ul <<  3),(1ul <<  4),
  (1ul <<  5),(1ul <<  6),(1ul <<  7),(1ul <<  8),(1ul <<  9),
  (1ul << 10),(1ul << 11),(1ul << 12),(1ul << 13),(1ul << 14),
  (1ul << 15),(1ul << 16),(1ul << 17),(1ul << 18),(1ul << 19),
  (1ul << 20),(1ul << 21),(1ul << 22),(1ul << 23),(1ul << 24),
  (1ul << 25),(1ul << 26),(1ul << 27),(1ul << 28),(1ul << 29),
  (1ul << 30),(1ul << 31),(1ul << 32),(1ul << 33),(1ul << 34),
  (1ul << 35),(1ul << 36),(1ul << 37),(1ul << 38),(1ul << 39),
  (1ul << 40),(1ul << 41),(1ul << 42),(1ul << 43),(1ul << 44),
  (1ul << 45),(1ul << 46),(1ul << 47),(1ul << 48),(1ul << 49),
  (1ul << 50),(1ul << 51),(1ul << 52),(1ul << 53),(1ul << 54),
  (1ul << 55),(1ul << 56),(1ul << 57),(1ul << 58),(1ul << 59),
  (1ul << 60),(1ul << 61),(1ul << 62),(1ul << 63)
};

// 0,1,2,3,3,2,1,0,
// 1,4,5,6,6,5,4,1,
// 2,5,7,8,8,7,5,2,
// 3,6,8,9,9,8,6,3,
// 3,6,8,9,9,8,6,3,
// 2,5,7,8,8,7,5,2,
// 1,4,5,6,6,5,4,1,
// 0,1,2,3,3,2,1,0

const std::bitset<64> board::location[10] = {
  /* 0 */ bit[0] | bit[7] | bit[56] | bit[63],
  /* 1 */ bit[1] | bit[6] | bit[8] | bit[15] | bit[48] | bit[55] | bit[57] | bit[62],
  /* 2 */ bit[2] | bit[5] | bit[16] | bit[23] | bit[40] | bit[47] | bit[58] | bit[61],
  /* 3 */ bit[3] | bit[4] | bit[24] | bit[31] | bit[32] | bit[39] | bit[59] | bit[60],
  /* 4 */ bit[9] | bit[14] | bit[49] | bit[54],
  /* 5 */ bit[10] | bit[13] | bit[17] | bit[22] | bit[41] | bit[46] | bit[50] | bit[53],
  /* 6 */ bit[11] | bit[12] | bit[25] | bit[30] | bit[33] | bit[38] | bit[51] | bit[52],
  /* 7 */ bit[18] | bit[21] | bit[42] | bit[45],
  /* 8 */ bit[19] | bit[20] | bit[26] | bit[29] | bit[34] | bit[37] | bit[43] | bit[44],
  /* 9 */ bit[27] | bit[28] | bit[35] | bit[36]
};
  






board* board::get_children(board* out_begin) const
{
  assert(out_begin);
  
  board* out_end = out_begin;
  std::bitset<64> valid_moves = get_valid_moves();
  
  while(true){
    
    int move_id = find_first_set_64(valid_moves.to_ulong());
    if(move_id == -1){
      break;
    } 
    
    *out_end = *this;
    out_end->do_move(move_id);
    out_end++;
    
    valid_moves.reset(move_id);
    
  }
  return out_end;
}



void board::show() const
{
  int x,y;
  
  std::bitset<64> black,white;
  black = (turn ? opp : me);
  white = (turn ? me : opp);
  
  
  
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
  else if(count[0] < count[1]){ /* Opp wins */
    return -64 + (2*count[0]);
  }
  else{ /* draw */
    return 0;
  }
}

void board::do_move(int move_id){
  assert(is_valid_move(move_id));
  
  
  
  std::bitset<64> tmp_mask,cur_bit;
  
  const std::bitset<64> move_bit = board::bit[move_id];
  
  for(int i=0;i<4;++i){
    
    tmp_mask.reset();
    cur_bit = move_bit;
    
     
    while(true){
      assert(cur_bit.any());
      
      // will i walk off the board next step?
      if(!(walk_possible[i][0] & cur_bit).any()){
        break;
      }
      
      
      cur_bit >>= board::walk_diff[7-i][0];
      
      // current field = my color
      if((me & cur_bit).any()){
        me |= tmp_mask;
        break;
      }
      
      // current field = opponent color
      if((opp & cur_bit).any()){
        tmp_mask |= cur_bit;
        continue;
      }
      
      // current fiend = empty
      break;
    }
  }
  for(int i=4;i<8;++i){
    
    tmp_mask.reset();
    cur_bit = move_bit;
    
    
    while(true){
      assert(cur_bit.any());
      
      // will i walk off the board next step?
      if(!(walk_possible[i][0] & cur_bit).any()){
        break;
      }
      
      cur_bit <<= board::walk_diff[i][0];
      
      // current field = my color
      if((me & cur_bit).any()){
        me |= tmp_mask;
        break;
      }
      
      // current field = opponent color
      if((opp & cur_bit).any()){
        tmp_mask |= cur_bit;
        continue;
      }
      
      // current fiend = empty
      break;
    }
  }
  
  
  me |= board::bit[move_id];
  opp &= ~me;
  
  passed = false;
  
  switch_turn();
}

void board::do_move(int move_id, std::bitset<64>* undo_data)
{
  assert(is_valid_move(move_id));
  
  
  undo_data->reset();
  
  std::bitset<64> tmp_mask,cur_bit;
  
  const std::bitset<64> move_bit = board::bit[move_id];
  
  for(int i=0;i<4;++i){
    
    tmp_mask.reset();
    cur_bit = move_bit;
    
    
    while(true){
      assert(cur_bit.any());
      
      // will i walk off the board next step?
      if(!(walk_possible[i][0] & cur_bit).any()){
        break;
      }
      
      
      cur_bit >>= board::walk_diff[7-i][0];
      
      // current field = my color
      if((me & cur_bit).any()){
        (*undo_data) |= tmp_mask;
        break;
      }
      
      // current field = opponent color
      if((opp & cur_bit).any()){
        tmp_mask |= cur_bit;
        continue;
      }
      
      // current fiend = empty
      break;
    }
  }
  for(int i=4;i<8;++i){
    
    tmp_mask.reset();
    cur_bit = move_bit;
    
    
    while(true){
      assert(cur_bit.any());
      
      // will i walk off the board next step?
      if(!(walk_possible[i][0] & cur_bit).any()){
        break;
      }
      
      cur_bit <<= board::walk_diff[i][0];
      
      // current field = my color
      if((me & cur_bit).any()){
        (*undo_data) |= tmp_mask;
        break;
      }
      
      // current field = opponent color
      if((opp & cur_bit).any()){
        tmp_mask |= cur_bit;
        continue;
      }
      
      // current fiend = empty
      break;
    }
  }
  
  assert((me & (*undo_data)).none());
  assert((opp & (*undo_data)) == (*undo_data));
  assert((get_non_empty_fields() & board::bit[move_id]).none());
  
  me |= ((*undo_data) | board::bit[move_id]);
  opp &= ~me;
  
  passed = false;
  
  switch_turn();
}



std::string board::to_string() const {
  std::string res;
  
  res += (turn ? 'o' : 'x');
  res += (passed ? 'o' : 'x');
  
  for(int i=0;i<64;i++){
    if(me.test(i)){
      res += 'o';
    }
    else if(opp.test(i)){
      res += 'x';
    }
    else{
      res += ' ';
    }
  }
  return res;
}

board::board(const std::string& in){
  turn = (in[0] == 'o');
  passed = (in[1] == 'o');

  reset();
  
  for(int i=2;i<66;i++){
    switch(in[i]){
      case 'o': 
        me.set(i-2);
        break;
      case 'x':
        opp.set(i-2);
        break;
      case ' ':
        // do nothing
        break;
      default:
        *(int*)(0) = 42;
    }
  }
}

