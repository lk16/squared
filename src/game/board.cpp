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

const bits64 board::walk_possible[8][7] = {
  { // up left
    0xFEFEFEFEFEFEFE00,
    0xFCFCFCFCFCFC0000,
    0xF8F8F8F8F8000000,
    0xF0F0F0F000000000,
    0xE0E0E00000000000,
    0xC0C0000000000000,
    0x8000000000000000
  },
  { // up
    0xFFFFFFFFFFFFFF00,
    0xFFFFFFFFFFFF0000,
    0xFFFFFFFFFF000000,
    0xFFFFFFFF00000000,
    0xFFFFFF0000000000,
    0xFFFF000000000000,
    0xFF00000000000000
  },
  { // up right
    0x7F7F7F7F7F7F7F00,
    0x3F3F3F3F3F3F0000,
    0x1F1F1F1F1F000000,
    0x0F0F0F0F00000000,
    0x0707070000000000,
    0x0303000000000000,
    0x0100000000000000
  },
  { // left
    0xFEFEFEFEFEFEFEFE,
    0xFCFCFCFCFCFCFCFC,
    0xF8F8F8F8F8F8F8F8,
    0xF0F0F0F0F0F0F0F0,
    0xE0E0E0E0E0E0E0E0,
    0xC0C0C0C0C0C0C0C0,
    0x8080808080808080
  },
  { // right
    0x7F7F7F7F7F7F7F7F,
    0x3F3F3F3F3F3F3F3F,
    0x1F1F1F1F1F1F1F1F,
    0x0F0F0F0F0F0F0F0F,
    0x0707070707070707,
    0x0303030303030303,
    0x0101010101010101
  },
  { // down left
    0x00FEFEFEFEFEFEFE,
    0x0000FCFCFCFCFCFC,
    0x000000F8F8F8F8F8,
    0x00000000F0F0F0F0,
    0x0000000000E0E0E0,
    0x000000000000C0C0,
    0x0000000000000080
  },
  { // down
    0x00FFFFFFFFFFFFFF,
    0x0000FFFFFFFFFFFF,
    0x000000FFFFFFFFFF,
    0x00000000FFFFFFFF,
    0x0000000000FFFFFF,
    0x000000000000FFFF,
    0x00000000000000FF
  },
  { // down right
    0x007F7F7F7F7F7F7F,
    0x00003F3F3F3F3F3F,
    0x0000001F1F1F1F1F,
    0x000000000F0F0F0F,
    0x0000000000070707,
    0x0000000000000303,
    0x0000000000000001
  }
};

bits64 (board::* const move_funcs[65])() = {
  &board::do_move_A1,
  &board::do_move_A2,
  &board::do_move_A3,
  &board::do_move_A4,
  &board::do_move_A5,
  &board::do_move_A6,
  &board::do_move_A7,
  &board::do_move_A8,
  &board::do_move_B1,
  &board::do_move_B2,
  &board::do_move_B3,
  &board::do_move_B4,
  &board::do_move_B5,
  &board::do_move_B6,
  &board::do_move_B7,
  &board::do_move_B8,
  &board::do_move_C1,
  &board::do_move_C2,
  &board::do_move_C3,
  &board::do_move_C4,
  &board::do_move_C5,
  &board::do_move_C6,
  &board::do_move_C7,
  &board::do_move_C8,
  &board::do_move_D1,
  &board::do_move_D2,
  &board::do_move_D3,
  &board::do_move_D4,
  &board::do_move_D5,
  &board::do_move_D6,
  &board::do_move_D7,
  &board::do_move_D8,
  &board::do_move_E1,
  &board::do_move_E2,
  &board::do_move_E3,
  &board::do_move_E4,
  &board::do_move_E5,
  &board::do_move_E6,
  &board::do_move_E7,
  &board::do_move_E8,
  &board::do_move_F1,
  &board::do_move_F2,
  &board::do_move_F3,
  &board::do_move_F4,
  &board::do_move_F5,
  &board::do_move_F6,
  &board::do_move_F7,
  &board::do_move_F8,
  &board::do_move_G1,
  &board::do_move_G2,
  &board::do_move_G3,
  &board::do_move_G4,
  &board::do_move_G5,
  &board::do_move_G6,
  &board::do_move_G7,
  &board::do_move_G8,
  &board::do_move_H1,
  &board::do_move_H2,
  &board::do_move_H3,
  &board::do_move_H4,
  &board::do_move_H5,
  &board::do_move_H6,
  &board::do_move_H7,
  &board::do_move_H8,
  &board::do_move_pass
};


// 0,1,2,3,3,2,1,0,
// 1,4,5,6,6,5,4,1,
// 2,5,7,8,8,7,5,2,
// 3,6,8,9,9,8,6,3,
// 3,6,8,9,9,8,6,3,
// 2,5,7,8,8,7,5,2,
// 1,4,5,6,6,5,4,1,
// 0,1,2,3,3,2,1,0

const bits64 board::location[10] = {
  /* 0 */ bits64_set[0] | bits64_set[7] | bits64_set[56] | bits64_set[63],
  /* 1 */ bits64_set[1] | bits64_set[6] | bits64_set[8] | bits64_set[15] | bits64_set[48] | bits64_set[55] | bits64_set[57] | bits64_set[62],
  /* 2 */ bits64_set[2] | bits64_set[5] | bits64_set[16] | bits64_set[23] | bits64_set[40] | bits64_set[47] | bits64_set[58] | bits64_set[61],
  /* 3 */ bits64_set[3] | bits64_set[4] | bits64_set[24] | bits64_set[31] | bits64_set[32] | bits64_set[39] | bits64_set[59] | bits64_set[60],
  /* 4 */ bits64_set[9] | bits64_set[14] | bits64_set[49] | bits64_set[54],
  /* 5 */ bits64_set[10] | bits64_set[13] | bits64_set[17] | bits64_set[22] | bits64_set[41] | bits64_set[46] | bits64_set[50] | bits64_set[53],
  /* 6 */ bits64_set[11] | bits64_set[12] | bits64_set[25] | bits64_set[30] | bits64_set[33] | bits64_set[38] | bits64_set[51] | bits64_set[52],
  /* 7 */ bits64_set[18] | bits64_set[21] | bits64_set[42] | bits64_set[45],
  /* 8 */ bits64_set[19] | bits64_set[20] | bits64_set[26] | bits64_set[29] | bits64_set[34] | bits64_set[37] | bits64_set[43] | bits64_set[44],
  /* 9 */ bits64_set[27] | bits64_set[28] | bits64_set[35] | bits64_set[36]
};
  

board* board::get_children(board* out_begin) const
{
  assert(out_begin);
  
  board* out_end = out_begin;
  bits64 valid_moves = get_valid_moves();
  
  while(true){
    
    int move_id = bits64_find_first(valid_moves);
    if(move_id == 64){
      break;
    } 
    
    *out_end = *this;
    out_end->do_move(move_id);
    out_end++;
    
    valid_moves &= bits64_reset[move_id];
    
  }
  return out_end;
}



void board::show() const
{
  int x,y;
  
  bits64 black,white;
  black = (true ? opp : me);
  white = (true ? me : opp);
  
  
  
  /* top line */
  std::cout << "+-----------------+\n";
  
  /* middle */
  for(y=0;y<8;y++){
    std::cout << "| ";
    for(x=0;x<8;x++){
      if(black & bits64_set[y*8+x]){
          std::cout << "\033[31;1m@\033[0m ";
      }
      else if(white & bits64_set[y*8+x]){
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
  
  count[0] = bits64_count(me);
  count[1] = bits64_count(opp);
  
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


bits64 board::do_move(int move_id)
{  
  
#if 1 
  return (this->*move_funcs[move_id])(); 
#else
  return do_move_experimental(move_id);
  
  bits64 tmp_mask,cur_bit,result = 0ull;
  
  for(int i=0;i<4;++i){
    
    tmp_mask = 0ull;
    cur_bit = bits64_set[move_id];
    
    
    while(true){
      
      // will i walk off the board next step?
      if((walk_possible[i][0] & cur_bit) == 0ull){
        break;
      }
      
      
      cur_bit >>= board::walk_diff[7-i][0];
      
      // current field = my color
      if((me & cur_bit) != 0ull){
        result |= tmp_mask;
        break;
      }
      
      // current field = opponent color
      if((opp & cur_bit) != 0ull){
        tmp_mask |= cur_bit;
        continue;
      }
      
      // current field = empty
      break;
    }
  }
  for(int i=4;i<8;++i){
    
    tmp_mask = 0ull;
    cur_bit = bits64_set[move_id];
    
    
    while(true){
      
      // will i walk off the board next step?
      if((walk_possible[i][0] & cur_bit) == 0ull){
        break;
      }
      
      cur_bit <<= board::walk_diff[i][0];
      
      // current field = my color
      if((me & cur_bit) != 0ull){
        result |= tmp_mask;
        break;
      }
      
      // current field = opponent color
      if((opp & cur_bit) != 0ull){
        tmp_mask |= cur_bit;
        continue;
      }
      
      // current fiend = empty
      break;
    }
  }
  
  assert((me & result) == 0ull);
  assert((opp & result) == result);
  assert((get_non_empty_fields() & bits64_set[move_id]) == 0ull);
  
  me |= (result | bits64_set[move_id]);
  opp &= ~me;
    
  switch_turn();
  
  return result;
#endif
}



std::string board::to_string() const {
  /* format:
   * byte 0-15: hex notation of me
   * byte 16-31: hex notation of opp
   * byte 32: \0
   */
  char res[33];  
  const char hex[17] = "0123456789abcdef";
  
  
  for(int i=0;i<16;i++){
    res[i] = hex[(me >> (4*i)) & 0xF];
    res[16+i] = hex[(opp >> (4*i)) & 0xF];
  }
  
  res[32] = '\0';
  
  return std::string(res);  
}

board::board(const std::string& in){
  
  try{
    if(in.length() != 32){
      throw 0;
    }
    
    opp = me = 0ull;
    
    unsigned long long x;
    for(int i=0;i<16;i++){
      if((in[i]>='0') && (in[i]<='9')){
        x = in[i]-'0';
      }
      else if((in[i]>='a') && (in[i]<='f')){
        x = 10 + in[i] - 'a';      
      }
      else{
        throw 1;
      }
      me |= ((x & 0xF) << (i*4));
    }
    
    for(int i=0;i<16;i++){
      if((in[16+i]>='0') && (in[16+i]<='9')){
        x = in[16+i]-'0';
      }
      else if((in[16+i]>='a') && (in[16+i]<='f')){
        x = 10 + in[16+i] - 'a';      
      }
      else{
        throw 2;
      }
      opp |= ((x & 0xF) << (i*4));
    }
  }
  catch(int i){
    me = opp = 0ull;
    std::cout << "ERROR: invalid board format fed to board(std::string): ";
    std::cout << in << "\n";
    std::cout << "Exception " << i << '\n';
    return;
    
  }
  
  
}

board board::do_random_moves(int count) const
{
  board moves[32];
  board* end = NULL;
  board res = *this;
  for(int i=0;i<count;i++){
    end = res.get_children(moves);
    if(end == moves){
      return res;
    }
    res = moves[rand() % (end-moves)];
  }
  return res;  
}

board board::rotate(int n) const
{
  board b(*this);
  
  b.me = bits64_rotate(b.me,n);
  b.opp = bits64_rotate(b.opp,n);
  
  return b;
}

std::string board::to_database_string() const
{
  return to_database_board().to_string(); 
}

board board::to_database_board() const
{
  board min = *this;
  
  for(int i=1;i<8;i++){
    board x = rotate(i);
    if(x.me == min.me){
      if(x.opp < min.opp){
        min = x;
      }
    }
    else if(x.me < min.me){
      min = x;
    }
  }
  
  return min; 
}


bits64 board::do_move_experimental(const int field_id){
  bits64 line,flipped = 0ull;
  int end;
  
  bits64 left_border_mask,right_border_mask;
  left_border_mask = right_border_mask = 0x0;
  
  switch(field_id%8){
    case 0: right_border_mask = 0xFEFEFEFEFEFEFEFE; break;
    case 1: right_border_mask = 0xFCFCFCFCFCFCFCFC; break;
    case 2: right_border_mask = 0xF8F8F8F8F8F8F8F8; break;
    case 3: right_border_mask = 0xF0F0F0F0F0F0F0F0; break;
    case 4: right_border_mask = 0xE0E0E0E0E0E0E0E0; break;
    case 5: right_border_mask = 0xC0C0C0C0C0C0C0C0; break;
    default: right_border_mask = 0x0; break;
  }
  
  switch(field_id%8){
    case 2: left_border_mask = 0x0303030303030303; break;
    case 3: left_border_mask = 0x0707070707070707; break;
    case 4: left_border_mask = 0x0F0F0F0F0F0F0F0F; break;
    case 5: left_border_mask = 0x1F1F1F1F1F1F1F1F; break;
    case 6: left_border_mask = 0x3F3F3F3F3F3F3F3F; break;
    case 7: left_border_mask = 0x7F7F7F7F7F7F7F7F; break;
    default: left_border_mask = 0x0; break;
  }
  
  /* down */
  if(field_id/8 < 6){
    line = 0x0101010101010100l << field_id;
    end = bits64_find_first(line & me);
    line &= bits64_before[end];
    if((opp & line) == line){
      flipped |= line;
    }
  }
  
  /* up */
  if(field_id/8 > 1){
    line = 0x0080808080808080l >> (63-field_id);
    end = bits64_find_last(line & me);
    line &= bits64_after[end];
    if((opp & line) == line){
      flipped |= line;
    }
  }
  
  /* left */
  if(field_id%8 > 1){
    line = (0x7F00000000000000l >> (63-field_id)) & left_border_mask;
    end = bits64_find_last(line & me);
    line &= bits64_after[end];
    if((opp & line) == line){
      flipped |= line;
    }
  }
  
  /* right */
  if(field_id%8 < 6){
    line = (0x00000000000000FEl << field_id) & right_border_mask;
    end = bits64_find_first(line & me);
    line &= bits64_before[end];
    if((opp & line) == line){
      flipped |= line;
    }
  }
  
  /* right down */
  if((field_id%8 < 6) && (field_id/8 < 6)){
    line = (0x8040201008040200 << field_id) & right_border_mask;
    end = bits64_find_first(line & me);
    line &= bits64_before[end];
    if((opp & line) == line){
      flipped |= line;
    }
  }
  
  /* left up */
  if((field_id%8 > 1) && (field_id/8 > 1)){
    line = (0x8040201008040200 >> (63-field_id)) & left_border_mask;
    end = bits64_find_last(line & me);
    line &= bits64_after[end];
    if((opp & line) == line){
      flipped |= line;
    }
  }
  
  /* right up */
  if((field_id%8 < 6) && (field_id/8 > 1)){
    if(field_id<=56){
      line = (0x0002040810204080 >> (56-field_id)) & right_border_mask;
    }
    else{
      line = (0x0002040810204080 << (field_id-56)) & right_border_mask;
    }
    end = bits64_find_last(line & me);
    line &= bits64_after[end];
     if((opp & line) == line){
      flipped |= line;
    }
  }
  
  /* left down */
  if((field_id%8 > 1) && (field_id/8 < 6)){
    if(field_id>=7){
      line = (0x0102040810204000 << (field_id-7)) & left_border_mask;
    }
    else{
      line = (0x0102040810204000 >> (7-field_id)) & left_border_mask;
    }
    end = bits64_find_first(line & me);
    line &= bits64_before[end];
    if((opp & line) == line){
      flipped |= line;
    }
  }
  
  me |= bits64_set[field_id] | flipped;
  opp &= ~me;
  switch_turn();  
  return flipped;
}

bits64 board::do_move_pass()
{
  switch_turn();
  return 0ull;
}


