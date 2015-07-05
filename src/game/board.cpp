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
  assert(out);
  
  while(moves != 0ull){
    int move_id = bits64_find_first(moves);
    *out = *this;
    out->do_move(move_id);
    out++;
    moves &= bits64_reset[move_id];
  }
  return out;  
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
    line = (0x0040201008040201 >> (63-field_id)) & left_border_mask;
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

void board::count_frontier_discs(int* me,int* opp) const
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
  
  *me = mask & this->me;
  *opp = mask & this->opp;
  
}
