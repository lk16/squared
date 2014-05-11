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

const bits board::walk_possible[8][7] = {
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

const bits board::bit[64] = {
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

const bits board::location[10] = {
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
  
const bits board::bits_before[64] = {
  0x0, // used to prevent warnings
  (0xFFFFFFFFFFFFFFFF << 63),
  (0xFFFFFFFFFFFFFFFF << 62),
  (0xFFFFFFFFFFFFFFFF << 61),
  (0xFFFFFFFFFFFFFFFF << 60),
  (0xFFFFFFFFFFFFFFFF << 59),
  (0xFFFFFFFFFFFFFFFF << 58),
  (0xFFFFFFFFFFFFFFFF << 57),
  (0xFFFFFFFFFFFFFFFF << 56),   
  (0xFFFFFFFFFFFFFFFF << 55),
  (0xFFFFFFFFFFFFFFFF << 54),
  (0xFFFFFFFFFFFFFFFF << 53),
  (0xFFFFFFFFFFFFFFFF << 52),
  (0xFFFFFFFFFFFFFFFF << 51),
  (0xFFFFFFFFFFFFFFFF << 50),
  (0xFFFFFFFFFFFFFFFF << 49),
  (0xFFFFFFFFFFFFFFFF << 48),
  (0xFFFFFFFFFFFFFFFF << 47),
  (0xFFFFFFFFFFFFFFFF << 46),   
  (0xFFFFFFFFFFFFFFFF << 45),
  (0xFFFFFFFFFFFFFFFF << 44),
  (0xFFFFFFFFFFFFFFFF << 43),
  (0xFFFFFFFFFFFFFFFF << 42),
  (0xFFFFFFFFFFFFFFFF << 41),
  (0xFFFFFFFFFFFFFFFF << 40),
  (0xFFFFFFFFFFFFFFFF << 39),
  (0xFFFFFFFFFFFFFFFF << 38),
  (0xFFFFFFFFFFFFFFFF << 37),
  (0xFFFFFFFFFFFFFFFF << 36),   
  (0xFFFFFFFFFFFFFFFF << 35),
  (0xFFFFFFFFFFFFFFFF << 34),
  (0xFFFFFFFFFFFFFFFF << 33),
  (0xFFFFFFFFFFFFFFFF << 32),
  (0xFFFFFFFFFFFFFFFF << 31),
  (0xFFFFFFFFFFFFFFFF << 30),
  (0xFFFFFFFFFFFFFFFF << 29),
  (0xFFFFFFFFFFFFFFFF << 28),
  (0xFFFFFFFFFFFFFFFF << 27),
  (0xFFFFFFFFFFFFFFFF << 26),   
  (0xFFFFFFFFFFFFFFFF << 25),
  (0xFFFFFFFFFFFFFFFF << 24),
  (0xFFFFFFFFFFFFFFFF << 23),
  (0xFFFFFFFFFFFFFFFF << 22),
  (0xFFFFFFFFFFFFFFFF << 21),
  (0xFFFFFFFFFFFFFFFF << 20),
  (0xFFFFFFFFFFFFFFFF << 19),
  (0xFFFFFFFFFFFFFFFF << 18),
  (0xFFFFFFFFFFFFFFFF << 17),
  (0xFFFFFFFFFFFFFFFF << 16),   
  (0xFFFFFFFFFFFFFFFF << 15),
  (0xFFFFFFFFFFFFFFFF << 14),
  (0xFFFFFFFFFFFFFFFF << 13),
  (0xFFFFFFFFFFFFFFFF << 12),
  (0xFFFFFFFFFFFFFFFF << 11),
  (0xFFFFFFFFFFFFFFFF << 10),
  (0xFFFFFFFFFFFFFFFF << 9),
  (0xFFFFFFFFFFFFFFFF << 8),
  (0xFFFFFFFFFFFFFFFF << 7),
  (0xFFFFFFFFFFFFFFFF << 6),   
  (0xFFFFFFFFFFFFFFFF << 5),
  (0xFFFFFFFFFFFFFFFF << 4),
  (0xFFFFFFFFFFFFFFFF << 3),
  (0xFFFFFFFFFFFFFFFF << 2),
  (0xFFFFFFFFFFFFFFFF << 1)
};

const bits board::bits_after[64] = {
  (0xFFFFFFFFFFFFFFFF << 1),
  (0xFFFFFFFFFFFFFFFF << 2),
  (0xFFFFFFFFFFFFFFFF << 3),
  (0xFFFFFFFFFFFFFFFF << 4),
  (0xFFFFFFFFFFFFFFFF << 5),
  (0xFFFFFFFFFFFFFFFF << 6),
  (0xFFFFFFFFFFFFFFFF << 7),
  (0xFFFFFFFFFFFFFFFF << 8),
  (0xFFFFFFFFFFFFFFFF << 9),
  (0xFFFFFFFFFFFFFFFF << 10),
  (0xFFFFFFFFFFFFFFFF << 11),
  (0xFFFFFFFFFFFFFFFF << 12),
  (0xFFFFFFFFFFFFFFFF << 13),
  (0xFFFFFFFFFFFFFFFF << 14),
  (0xFFFFFFFFFFFFFFFF << 15),
  (0xFFFFFFFFFFFFFFFF << 16),
  (0xFFFFFFFFFFFFFFFF << 17),
  (0xFFFFFFFFFFFFFFFF << 18),
  (0xFFFFFFFFFFFFFFFF << 19),
  (0xFFFFFFFFFFFFFFFF << 20),
  (0xFFFFFFFFFFFFFFFF << 21),
  (0xFFFFFFFFFFFFFFFF << 22),
  (0xFFFFFFFFFFFFFFFF << 23),
  (0xFFFFFFFFFFFFFFFF << 24),
  (0xFFFFFFFFFFFFFFFF << 25),
  (0xFFFFFFFFFFFFFFFF << 26),
  (0xFFFFFFFFFFFFFFFF << 27),
  (0xFFFFFFFFFFFFFFFF << 28),
  (0xFFFFFFFFFFFFFFFF << 29),
  (0xFFFFFFFFFFFFFFFF << 30),
  (0xFFFFFFFFFFFFFFFF << 31),
  (0xFFFFFFFFFFFFFFFF << 32),
  (0xFFFFFFFFFFFFFFFF << 33),
  (0xFFFFFFFFFFFFFFFF << 34),
  (0xFFFFFFFFFFFFFFFF << 35),
  (0xFFFFFFFFFFFFFFFF << 36),
  (0xFFFFFFFFFFFFFFFF << 37),
  (0xFFFFFFFFFFFFFFFF << 38),
  (0xFFFFFFFFFFFFFFFF << 39),
  (0xFFFFFFFFFFFFFFFF << 40),
  (0xFFFFFFFFFFFFFFFF << 41),
  (0xFFFFFFFFFFFFFFFF << 42),
  (0xFFFFFFFFFFFFFFFF << 43),
  (0xFFFFFFFFFFFFFFFF << 44),
  (0xFFFFFFFFFFFFFFFF << 45),
  (0xFFFFFFFFFFFFFFFF << 46),
  (0xFFFFFFFFFFFFFFFF << 47),
  (0xFFFFFFFFFFFFFFFF << 48),
  (0xFFFFFFFFFFFFFFFF << 49),
  (0xFFFFFFFFFFFFFFFF << 50),
  (0xFFFFFFFFFFFFFFFF << 51),
  (0xFFFFFFFFFFFFFFFF << 52),
  (0xFFFFFFFFFFFFFFFF << 53),
  (0xFFFFFFFFFFFFFFFF << 54),
  (0xFFFFFFFFFFFFFFFF << 55),
  (0xFFFFFFFFFFFFFFFF << 56),
  (0xFFFFFFFFFFFFFFFF << 57),
  (0xFFFFFFFFFFFFFFFF << 58),
  (0xFFFFFFFFFFFFFFFF << 59),
  (0xFFFFFFFFFFFFFFFF << 60),
  (0xFFFFFFFFFFFFFFFF << 61),
  (0xFFFFFFFFFFFFFFFF << 62),
  (0xFFFFFFFFFFFFFFFF << 63),
  0x0 // used to prevent compiler warnings
};




board* board::get_children(board* out_begin) const
{
  assert(out_begin);
  
  board* out_end = out_begin;
  bits valid_moves = get_valid_moves();
  
  while(true){
    
    int move_id = find_first_set_64(valid_moves);
    if(move_id == -1){
      break;
    } 
    
    *out_end = *this;
    out_end->do_move(move_id);
    out_end++;
    
    valid_moves &= ~board::bit[move_id];
    
  }
  return out_end;
}



void board::show() const
{
  int x,y;
  
  bits black,white;
  black = (turn ? opp : me);
  white = (turn ? me : opp);
  
  
  
  /* top line */
  std::cout << "+-----------------+\n";
  
  /* middle */
  for(y=0;y<8;y++){
    std::cout << "| ";
    for(x=0;x<8;x++){
      if(black & board::bit[y*8+x]){
          std::cout << "\033[31;1m@\033[0m ";
      }
      else if(white & board::bit[y*8+x]){
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
  
  count[0] = count_64(me);
  count[1] = count_64(opp);
  
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


bits board::do_move(int move_id)
{
  assert(is_valid_move(move_id));
  
  bits tmp_mask,cur_bit,result = 0ull;
  
  for(int i=0;i<4;++i){
    
    tmp_mask = 0ull;
    cur_bit = board::bit[move_id];
    
    
    while(true){
      assert(cur_bit != 0ull);
      
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
    cur_bit = board::bit[move_id];
    
    
    while(true){
      assert(cur_bit.any());
      
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
  
  assert((me & result).none());
  assert((opp & result) == result);
  assert((get_non_empty_fields() & board::bit[move_id]).none());
  
  me |= (result | board::bit[move_id]);
  opp &= ~me;
  
  passed = false;
  
  switch_turn();
  
  return result;
}



std::string board::to_string() const {
  /* format:
   * byte 0: '0' + bitset of turn = 0x1, passed = 0x2
   * byte 1: reserved for rotation purposes
   * byte 2-17: hex notation of me
   * byte 18-33: hex notation of opp
   * byte 34: \0
   */
  char res[35];  
  const char hex[17] = "0123456789abcdef";
  
  res[0] = '0' + ((turn ? 0x1 : 0x0) | (passed ? 0x2 : 0x0));
  res[1] = '0';
  
  for(int i=0;i<16;i++){
    res[2+i] = hex[(me >> (4*i)) & 0xF];
    res[18+i] = hex[(opp >> (4*i)) & 0xF];
  }
  
  res[34] = '\0';
  
  return std::string(res);  
}

board::board(const std::string& in){
  
  try{
    if(in.length() != 34){
      throw 0;
    }
    
    turn = ((in[0] - '0') &  0x1);
    passed = ((in[0] - '0') & 0x2);

    opp = me = 0ull;
    
    unsigned long long x;
    for(int i=0;i<16;i++){
      if((in[2+i]>='0') && (in[2+i]<='9')){
        x = in[2+i]-'0';
      }
      else if((in[2+i]>='a') && (in[2+i]<='f')){
        x = 10 + in[2+i] - 'a';      
      }
      else{
        throw 1;
      }
      me |= ((x & 0xF) << (i*4));
    }
    
    for(int i=0;i<16;i++){
      if((in[18+i]>='0') && (in[18+i]<='9')){
        x = in[18+i]-'0';
      }
      else if((in[18+i]>='a') && (in[18+i]<='f')){
        x = 10 + in[18+i] - 'a';      
      }
      else{
        throw 2;
      }
      opp |= ((x & 0xF) << (i*4));
    }
  }
  catch(int i){
    me = opp = 0ull;
    turn = passed = false;
    std::cout << "ERROR: invalid board format fed to board(std::string): ";
    std::cout << in << "\n";
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
  
  if(n>7){
    std::cout << "WARNING: invalid n (" << n << ") fed to board::rotate!\n";
    return b;
  }
  
  if(n>=4){
    b.me = mirror_vertical_line(b.me);
    b.opp = mirror_vertical_line(b.opp);
    n -= 4;
  }
  
  for(int i=0;i<n;i++){
    b.me = rotate_left(b.me);
    b.opp = rotate_left(b.opp);
  }
  
  return b;
}

std::string board::to_database_string() const
{
  std::string min_str = to_string();
  
  for(int i=1;i<8;i++){
    std::string str = rotate(i).to_string();
    min_str = (str < min_str) ? str : min_str;
  }
  
  return min_str; 
}

bits board::do_move_field_id(int field_id){
  bits flipped = 0ull,mask;
  int end;
  // down
  if(field_id/8 < 6){
    mask = 0x0010101010101010 << field_id;
    end = find_first_set_64(mask & me);
    if((opp & mask) == (bits_before[end] & mask)){
      flipped |= (opp & mask);
    }
  }
  
  me |= bit[field_id] | flipped;
  opp &= ~me;
  return flipped;
}

