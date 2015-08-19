#include "board.hpp"

bits64 (board::* const move_funcs[64])() = {
  &board::do_move_internal<board::A1>,
  &board::do_move_internal<board::A2>,
  &board::do_move_internal<board::A3>,
  &board::do_move_internal<board::A4>,
  &board::do_move_internal<board::A5>,
  &board::do_move_internal<board::A6>,
  &board::do_move_internal<board::A7>,
  &board::do_move_internal<board::A8>,
  &board::do_move_internal<board::B1>,
  &board::do_move_internal<board::B2>,
  &board::do_move_internal<board::B3>,
  &board::do_move_internal<board::B4>,
  &board::do_move_internal<board::B5>,
  &board::do_move_internal<board::B6>,
  &board::do_move_internal<board::B7>,
  &board::do_move_internal<board::B8>,
  &board::do_move_internal<board::C1>,
  &board::do_move_internal<board::C2>,
  &board::do_move_internal<board::C3>,
  &board::do_move_internal<board::C4>,
  &board::do_move_internal<board::C5>,
  &board::do_move_internal<board::C6>,
  &board::do_move_internal<board::C7>,
  &board::do_move_internal<board::C8>,
  &board::do_move_internal<board::D1>,
  &board::do_move_internal<board::D2>,
  &board::do_move_internal<board::D3>,
  &board::do_move_internal<board::D4>,
  &board::do_move_internal<board::D5>,
  &board::do_move_internal<board::D6>,
  &board::do_move_internal<board::D7>,
  &board::do_move_internal<board::D8>,
  &board::do_move_internal<board::E1>,
  &board::do_move_internal<board::E2>,
  &board::do_move_internal<board::E3>,
  &board::do_move_internal<board::E4>,
  &board::do_move_internal<board::E5>,
  &board::do_move_internal<board::E6>,
  &board::do_move_internal<board::E7>,
  &board::do_move_internal<board::E8>,
  &board::do_move_internal<board::F1>,
  &board::do_move_internal<board::F2>,
  &board::do_move_internal<board::F3>,
  &board::do_move_internal<board::F4>,
  &board::do_move_internal<board::F5>,
  &board::do_move_internal<board::F6>,
  &board::do_move_internal<board::F7>,
  &board::do_move_internal<board::F8>,
  &board::do_move_internal<board::G1>,
  &board::do_move_internal<board::G2>,
  &board::do_move_internal<board::G3>,
  &board::do_move_internal<board::G4>,
  &board::do_move_internal<board::G5>,
  &board::do_move_internal<board::G6>,
  &board::do_move_internal<board::G7>,
  &board::do_move_internal<board::G8>,
  &board::do_move_internal<board::H1>,
  &board::do_move_internal<board::H2>,
  &board::do_move_internal<board::H3>,
  &board::do_move_internal<board::H4>,
  &board::do_move_internal<board::H5>,
  &board::do_move_internal<board::H6>,
  &board::do_move_internal<board::H7>,
  &board::do_move_internal<board::H8>
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

// 0,1,2,3,3,2,1,0,
// 1,4,5,6,6,5,4,1,
// 2,5,7,8,8,7,5,2,
// 3,6,8,9,9,8,6,3,
// 3,6,8,9,9,8,6,3,
// 2,5,7,8,8,7,5,2,
// 1,4,5,6,6,5,4,1,
// 0,1,2,3,3,2,1,0

const bits64 board::ordered_locations[10] = {
  location[0],
  location[8],
  location[7],
  location[3],
  location[6],
  location[5],
  location[2],
  location[1],
  location[4],
  location[9]
  
};



void board::xot()
{
  int xot_boards_size = sizeof(xot_boards) / sizeof(xot_boards[0]);
  int index = rand() % xot_boards_size;
  me = xot_boards[index][0];
  opp = xot_boards[index][1];
}


board* board::get_children(board* out_begin) const
{
  return get_children(out_begin,get_valid_moves());
}

board* board::get_children(board* out, bits64 moves) const
{  
  while(moves != 0ull){
    bits64 bit = bits64_first(moves);
    int index = bits64_only_bit_index(bit);
    *out = *this;
    out->do_move(index);
    out++;
    moves ^= bit;
  }
  return out;  
}

int board::get_mobility(bits64 moves) const
{
  board copy;
  int total = 0;
  while(moves != 0ull){
    copy = *this;
    bits64 bit = bits64_first(moves);
    int index = bits64_only_bit_index(bit);
    total += bits64_count(copy.do_move(index));
    moves ^= bit;
  }
  return total;
}


bool board::only_similar_siblings(const board* siblings, int n)
{
  const board x = siblings[0].to_database_board();
  
  for(int i=1;i<n;i++){
    if(siblings[i].to_database_board() != x){
      return false;
    }
  }
  return true;  
}

int board::get_disc_diff() const
{
  
  int me_count = bits64_count(me);
  int opp_count = bits64_count(opp);
  
  int diff = me_count - opp_count;
  int empties = 64 - me_count - opp_count;
  if(diff > 0){
    return diff + empties;
  }
  else if(diff < 0){
    return diff - empties;
  }
  return diff;
}

std::string board::to_html_table(int turn) const
{
  std::stringstream ss;
    
  std::string colors[2] = {"black","white"};
  
  
  bits64 moves = get_valid_moves();
  
  // top line 
  ss << "<table border='0' cellspacing='0' cellpadding='0'><tr>\n";
  ss << "<td></td>\n";
  for(int i=0;i<8;i++){
    ss << "<td align='center'>" << (char)('a'+i) << "</td>\n";
  }
  ss << "</tr>";
  
  
  for(int f=0;f<64;f++){
    
    // left line
    if(f%8 == 0){
      ss << "<tr><td width='15' align='center'>" << (char)('1'+(f/8)) << "</td>";
    }
    
    bits64 thisbit = bits64_set[f];
    ss << "<td><img src='" << IMAGE_PATH << "small/";
    
    if(me & thisbit){
      ss << colors[turn] << ".png'>";
    }
    else if(opp & thisbit){
      ss << colors[1-turn] << ".png'>";
    }
    else if(moves & thisbit){
      
      ss << "move_" << colors[turn] << ".png'>";
    }  
    else{
      ss << "empty.png'>";
    }
    
    ss << "</td>";
    
    // right line
    if(f%8 == 7){
      ss << "</tr>";
    }
  }
  
  // bottom line
  ss << "</table>\n";
  
  return ss.str();
}




std::string board::to_ascii_art(int turn) const
{
  std::stringstream ss;
  
  bits64 x,y;
  
  if(turn==0){
    x = me;
    y = opp;
  }
  else{
    x = opp;
    y = me;
  }
  
  bits64 moves = get_valid_moves();
  
  // top line 
  ss << "+-a-b-c-d-e-f-g-h-+\n";
  
  for(int f=0;f<64;f++){
    
    // left line
    if(f%8 == 0){
      ss << (f/8)+1 << ' ';
    }
    
    bits64 thisbit = bits64_set[f];
    
    if(x & thisbit){
      ss << "\033[31;1m\u2B24\033[0m ";
    }
    else if(y & thisbit){
      ss << "\033[34;1m\u2B24\033[0m ";
    }
    else if(moves & thisbit){
      ss << "- ";
    }  
    else{
      ss << "  ";
    }
    
    // right line
    if(f%8 == 7){
      ss << "|\n";
    }
  }
  
  // bottom line
  ss << "+-----------------+\n";
  
  return ss.str();
}


int board::position_to_index(const std::string& str)
{
  if(str == "--"){
    return -1;
  }
  if(str.length()==2 && in_bounds(str[0],'a','h') && in_bounds(str[1],'1','8')){
    return 8*(str[1] - '1') + str[0] - 'a';
  }
  return -2;
}

std::string board::index_to_position(int index)
{
  if(out_bounds(index,0,63)){
    return "??";
  }
  char s[3];
  s[0] = 'a' + (index%8);
  s[1] = '1' + (index/8);
  s[2] = '\0';
  return std::string(s);
}


std::string board::to_string() const {
  /* format:
   * byte 0-15: hex notation of me
   * byte 16-31: hex notation of opp
   * byte 32: \0
   */
  char res[33];  
  const char* hex = "0123456789abcdef";
  
  
  for(int i=0;i<16;i++){
    res[i] = hex[(me >> (4*i)) & 0xF];
    res[16+i] = hex[(opp >> (4*i)) & 0xF];
  }
  
  res[32] = '\0';
  
  return std::string(res);  
}

board::board(const std::string& in){

  opp = me = 0ull;
  bool error = false;
  if(in.length() != 32){
    error = true;
  }
  for(unsigned i=0;i<in.length();++i){
    if(out_bounds(in[i],'0','9') && out_bounds(in[i],'a','f')){
      error = true;
    }
  }
  
  if(error){
    std::cerr << "ERROR in board::board(): invalid string: \"" << in << "\".\n";
    return;
  }
  
  
  unsigned long long x;
  for(int i=0;i<16;i++){
    if((in[i]>='0') && (in[i]<='9')){
      x = in[i]-'0';
    }
    else{
      x = 10 + in[i] - 'a';      
    }
    me |= ((x & 0xF) << (i*4));
  }
  
  for(int i=0;i<16;i++){
    if((in[16+i]>='0') && (in[16+i]<='9')){
      x = in[16+i]-'0';
    }
    else{
      x = 10 + in[16+i] - 'a';      
    }
    opp |= ((x & 0xF) << (i*4));
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

void board::get_frontier_discs(int* _me,int* _opp) const
{
  bits64 both = get_non_empty_fields();
  bits64 mask = both;
  
  mask &= ((both << 9) & 0xFEFEFEFEFEFEFEFE);
  mask &= ((both << 8));
  mask &= ((both << 7) & 0x7F7F7F7F7F7F7F7F);
  mask &= ((both << 1) & 0xFEFEFEFEFEFEFEFE);
  mask &= ((both >> 1) & 0x7F7F7F7F7F7F7F7F);
  mask &= ((both >> 7) & 0xFEFEFEFEFEFEFEFE);
  mask &= ((both >> 8));
  mask &= ((both >> 9) & 0x7F7F7F7F7F7F7F7F);
  
  *_me = mask & this->me;
  *_opp = mask & this->opp;
  
}

const bits64 board::dir_mask[64][8] = 
{
  {
    0x0,
    0x0,
    0x0,
    0x0,
    0xfe,
    0x0,
    0x101010101010100,
    0x8040201008040200,
  },
  {
    0x0,
    0x0,
    0x0,
    0x0,
    0xfc,
    0x0,
    0x202020202020200,
    0x80402010080400,
  },
  {
    0x0,
    0x0,
    0x0,
    0x3,
    0xf8,
    0x10200,
    0x404040404040400,
    0x804020100800,
  },
  {
    0x0,
    0x0,
    0x0,
    0x7,
    0xf0,
    0x1020400,
    0x808080808080800,
    0x8040201000,
  },
  {
    0x0,
    0x0,
    0x0,
    0xf,
    0xe0,
    0x102040800,
    0x1010101010101000,
    0x80402000,
  },
  {
    0x0,
    0x0,
    0x0,
    0x1f,
    0xc0,
    0x10204081000,
    0x2020202020202000,
    0x804000,
  },
  {
    0x0,
    0x0,
    0x0,
    0x3f,
    0x0,
    0x1020408102000,
    0x4040404040404000,
    0x0,
  },
  {
    0x0,
    0x0,
    0x0,
    0x7f,
    0x0,
    0x102040810204000,
    0x8080808080808000,
    0x0,
  },
  {
    0x0,
    0x0,
    0x0,
    0x0,
    0xfe00,
    0x0,
    0x101010101010000,
    0x4020100804020000,
  },
  {
    0x0,
    0x0,
    0x0,
    0x0,
    0xfc00,
    0x0,
    0x202020202020000,
    0x8040201008040000,
  },
  {
    0x0,
    0x0,
    0x0,
    0x300,
    0xf800,
    0x1020000,
    0x404040404040000,
    0x80402010080000,
  },
  {
    0x0,
    0x0,
    0x0,
    0x700,
    0xf000,
    0x102040000,
    0x808080808080000,
    0x804020100000,
  },
  {
    0x0,
    0x0,
    0x0,
    0xf00,
    0xe000,
    0x10204080000,
    0x1010101010100000,
    0x8040200000,
  },
  {
    0x0,
    0x0,
    0x0,
    0x1f00,
    0xc000,
    0x1020408100000,
    0x2020202020200000,
    0x80400000,
  },
  {
    0x0,
    0x0,
    0x0,
    0x3f00,
    0x0,
    0x102040810200000,
    0x4040404040400000,
    0x0,
  },
  {
    0x0,
    0x0,
    0x0,
    0x7f00,
    0x0,
    0x204081020400000,
    0x8080808080800000,
    0x0,
  },
  {
    0x0,
    0x101,
    0x204,
    0x0,
    0xfe0000,
    0x0,
    0x101010101000000,
    0x2010080402000000,
  },
  {
    0x0,
    0x202,
    0x408,
    0x0,
    0xfc0000,
    0x0,
    0x202020202000000,
    0x4020100804000000,
  },
  {
    0x201,
    0x404,
    0x810,
    0x30000,
    0xf80000,
    0x102000000,
    0x404040404000000,
    0x8040201008000000,
  },
  {
    0x402,
    0x808,
    0x1020,
    0x70000,
    0xf00000,
    0x10204000000,
    0x808080808000000,
    0x80402010000000,
  },
  {
    0x804,
    0x1010,
    0x2040,
    0xf0000,
    0xe00000,
    0x1020408000000,
    0x1010101010000000,
    0x804020000000,
  },
  {
    0x1008,
    0x2020,
    0x4080,
    0x1f0000,
    0xc00000,
    0x102040810000000,
    0x2020202020000000,
    0x8040000000,
  },
  {
    0x2010,
    0x4040,
    0x0,
    0x3f0000,
    0x0,
    0x204081020000000,
    0x4040404040000000,
    0x0,
  },
  {
    0x4020,
    0x8080,
    0x0,
    0x7f0000,
    0x0,
    0x408102040000000,
    0x8080808080000000,
    0x0,
  },
  {
    0x0,
    0x10101,
    0x20408,
    0x0,
    0xfe000000,
    0x0,
    0x101010100000000,
    0x1008040200000000,
  },
  {
    0x0,
    0x20202,
    0x40810,
    0x0,
    0xfc000000,
    0x0,
    0x202020200000000,
    0x2010080400000000,
  },
  {
    0x20100,
    0x40404,
    0x81020,
    0x3000000,
    0xf8000000,
    0x10200000000,
    0x404040400000000,
    0x4020100800000000,
  },
  {
    0x40201,
    0x80808,
    0x102040,
    0x7000000,
    0xf0000000,
    0x1020400000000,
    0x808080800000000,
    0x8040201000000000,
  },
  {
    0x80402,
    0x101010,
    0x204080,
    0xf000000,
    0xe0000000,
    0x102040800000000,
    0x1010101000000000,
    0x80402000000000,
  },
  {
    0x100804,
    0x202020,
    0x408000,
    0x1f000000,
    0xc0000000,
    0x204081000000000,
    0x2020202000000000,
    0x804000000000,
  },
  {
    0x201008,
    0x404040,
    0x0,
    0x3f000000,
    0x0,
    0x408102000000000,
    0x4040404000000000,
    0x0,
  },
  {
    0x402010,
    0x808080,
    0x0,
    0x7f000000,
    0x0,
    0x810204000000000,
    0x8080808000000000,
    0x0,
  },
  {
    0x0,
    0x1010101,
    0x2040810,
    0x0,
    0xfe00000000,
    0x0,
    0x101010000000000,
    0x804020000000000,
  },
  {
    0x0,
    0x2020202,
    0x4081020,
    0x0,
    0xfc00000000,
    0x0,
    0x202020000000000,
    0x1008040000000000,
  },
  {
    0x2010000,
    0x4040404,
    0x8102040,
    0x300000000,
    0xf800000000,
    0x1020000000000,
    0x404040000000000,
    0x2010080000000000,
  },
  {
    0x4020100,
    0x8080808,
    0x10204080,
    0x700000000,
    0xf000000000,
    0x102040000000000,
    0x808080000000000,
    0x4020100000000000,
  },
  {
    0x8040201,
    0x10101010,
    0x20408000,
    0xf00000000,
    0xe000000000,
    0x204080000000000,
    0x1010100000000000,
    0x8040200000000000,
  },
  {
    0x10080402,
    0x20202020,
    0x40800000,
    0x1f00000000,
    0xc000000000,
    0x408100000000000,
    0x2020200000000000,
    0x80400000000000,
  },
  {
    0x20100804,
    0x40404040,
    0x0,
    0x3f00000000,
    0x0,
    0x810200000000000,
    0x4040400000000000,
    0x0,
  },
  {
    0x40201008,
    0x80808080,
    0x0,
    0x7f00000000,
    0x0,
    0x1020400000000000,
    0x8080800000000000,
    0x0,
  },
  {
    0x0,
    0x101010101,
    0x204081020,
    0x0,
    0xfe0000000000,
    0x0,
    0x101000000000000,
    0x402000000000000,
  },
  {
    0x0,
    0x202020202,
    0x408102040,
    0x0,
    0xfc0000000000,
    0x0,
    0x202000000000000,
    0x804000000000000,
  },
  {
    0x201000000,
    0x404040404,
    0x810204080,
    0x30000000000,
    0xf80000000000,
    0x102000000000000,
    0x404000000000000,
    0x1008000000000000,
  },
  {
    0x402010000,
    0x808080808,
    0x1020408000,
    0x70000000000,
    0xf00000000000,
    0x204000000000000,
    0x808000000000000,
    0x2010000000000000,
  },
  {
    0x804020100,
    0x1010101010,
    0x2040800000,
    0xf0000000000,
    0xe00000000000,
    0x408000000000000,
    0x1010000000000000,
    0x4020000000000000,
  },
  {
    0x1008040201,
    0x2020202020,
    0x4080000000,
    0x1f0000000000,
    0xc00000000000,
    0x810000000000000,
    0x2020000000000000,
    0x8040000000000000,
  },
  {
    0x2010080402,
    0x4040404040,
    0x0,
    0x3f0000000000,
    0x0,
    0x1020000000000000,
    0x4040000000000000,
    0x0,
  },
  {
    0x4020100804,
    0x8080808080,
    0x0,
    0x7f0000000000,
    0x0,
    0x2040000000000000,
    0x8080000000000000,
    0x0,
  },
  {
    0x0,
    0x10101010101,
    0x20408102040,
    0x0,
    0xfe000000000000,
    0x0,
    0x0,
    0x0,
  },
  {
    0x0,
    0x20202020202,
    0x40810204080,
    0x0,
    0xfc000000000000,
    0x0,
    0x0,
    0x0,
  },
  {
    0x20100000000,
    0x40404040404,
    0x81020408000,
    0x3000000000000,
    0xf8000000000000,
    0x0,
    0x0,
    0x0,
  },
  {
    0x40201000000,
    0x80808080808,
    0x102040800000,
    0x7000000000000,
    0xf0000000000000,
    0x0,
    0x0,
    0x0,
  },
  {
    0x80402010000,
    0x101010101010,
    0x204080000000,
    0xf000000000000,
    0xe0000000000000,
    0x0,
    0x0,
    0x0,
  },
  {
    0x100804020100,
    0x202020202020,
    0x408000000000,
    0x1f000000000000,
    0xc0000000000000,
    0x0,
    0x0,
    0x0,
  },
  {
    0x201008040201,
    0x404040404040,
    0x0,
    0x3f000000000000,
    0x0,
    0x0,
    0x0,
    0x0,
  },
  {
    0x402010080402,
    0x808080808080,
    0x0,
    0x7f000000000000,
    0x0,
    0x0,
    0x0,
    0x0,
  },
  {
    0x0,
    0x1010101010101,
    0x2040810204080,
    0x0,
    0xfe00000000000000,
    0x0,
    0x0,
    0x0,
  },
  {
    0x0,
    0x2020202020202,
    0x4081020408000,
    0x0,
    0xfc00000000000000,
    0x0,
    0x0,
    0x0,
  },
  {
    0x2010000000000,
    0x4040404040404,
    0x8102040800000,
    0x300000000000000,
    0xf800000000000000,
    0x0,
    0x0,
    0x0,
  },
  {
    0x4020100000000,
    0x8080808080808,
    0x10204080000000,
    0x700000000000000,
    0xf000000000000000,
    0x0,
    0x0,
    0x0,
  },
  {
    0x8040201000000,
    0x10101010101010,
    0x20408000000000,
    0xf00000000000000,
    0xe000000000000000,
    0x0,
    0x0,
    0x0,
  },
  {
    0x10080402010000,
    0x20202020202020,
    0x40800000000000,
    0x1f00000000000000,
    0xc000000000000000,
    0x0,
    0x0,
    0x0,
  },
  {
    0x20100804020100,
    0x40404040404040,
    0x0,
    0x3f00000000000000,
    0x0,
    0x0,
    0x0,
    0x0,
  },
  {
    0x40201008040201,
    0x80808080808080,
    0x0,
    0x7f00000000000000,
    0x0,
    0x0,
    0x0,
    0x0,
  }
};
