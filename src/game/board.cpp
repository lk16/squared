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
  
const std::bitset<64> board::capture[8][6] = {
  {
    0x0000000000000200,
    0x0000000000020400,
    0x0000000002040800,
    0x0000000204081000,
    0x0000020408102000,
    0x0002040810204000
  },
  {
    0x0000000000000100,
    0x0000000000010100,
    0x0000000001010100,
    0x0000000101010100,
    0x0000010101010100,
    0x0001010101010100
  },
  {
    0x0000000000000200,
    0x0000000000040200,
    0x0000000008040200,
    0x0000001008040200,
    0x0000201008040200,
    0x0040201008040200
  },
  {
    0x0000000000000002,
    0x0000000000000006,
    0x000000000000000e,
    0x000000000000001e,
    0x000000000000003e,
    0x000000000000007e
  },
  {
    0x0000000000000002,
    0x0000000000000006,
    0x000000000000000E,
    0x000000000000001E,
    0x000000000000003E,
    0x000000000000007E
  },
  {
    0x0000000000000200,
    0x0000000000040200,
    0x0000000008040200,
    0x0000001008040200,
    0x0000201008040200,
    0x0040201008040200
  },
  {
    0x0000000000000100,
    0x0000000000010100,
    0x0000000001010100,
    0x0000000101010100,
    0x0000010101010100,
    0x0001010101010100
  },
  {
    0x0000000000000200,
    0x0000000000020400,
    0x0000000002040800,
    0x0000000204081000,
    0x0000020408102000,
    0x0002040810204000
  }
};  

const int board::capture_start[8][6] = {
  { 2, 3, 4, 5, 6, 7}, // down left
  { 0, 0, 0, 0, 0, 0}, // down
  { 0, 0, 0, 0, 0, 0}, // down right
  { 2, 3, 4, 5, 6, 7}, // left
  { 0, 0, 0, 0, 0, 0}, // right
  {18,27,36,45,54,63}, // up left
  {16,24,32,40,48,56}, // up 
  {16,24,32,40,48,56}  // up right
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
  
  for(int d=4;d<8;d++){
    
    assert(board::walk_diff[d][0] > 0);
    
    *out |= 
    (
      ((opp >> board::walk_diff[d][0]) & board::walk_possible[d][0]) 
      & 
      (
        ((me >> board::walk_diff[d][1]) & board::walk_possible[d][1])
        |
        (
          ((opp >> board::walk_diff[d][1]) & board::walk_possible[d][1])
          &
          (
            ((me >> board::walk_diff[d][2]) & board::walk_possible[d][2])
            |
            (
              ((opp >> board::walk_diff[d][2]) & board::walk_possible[d][2])
              &
              (
                ((me >> board::walk_diff[d][3]) & board::walk_possible[d][3])
                |
                (
                  ((opp >> board::walk_diff[d][3]) & board::walk_possible[d][3])
                  &
                  (
                    ((me >> board::walk_diff[d][4]) & board::walk_possible[d][4])
                    |
                    (
                      ((opp >> board::walk_diff[d][4]) & board::walk_possible[d][4])
                      &
                      (
                        ((me >> board::walk_diff[d][5]) & board::walk_possible[d][5])
                        |
                        (
                          ((opp >> board::walk_diff[d][5]) & board::walk_possible[d][5])
                          &
                          ((me >> board::walk_diff[d][6]) & board::walk_possible[d][6])
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
      ((opp << board::walk_diff[d][0]) & board::walk_possible[7-d][0]) 
      & 
      (
        ((me << board::walk_diff[d][1]) & board::walk_possible[7-d][1])
        |
        (
          ((opp << board::walk_diff[d][1]) & board::walk_possible[7-d][1])
          &
          (
            ((me << board::walk_diff[d][2]) & board::walk_possible[7-d][2])
            |
            (
              ((opp << board::walk_diff[d][2]) & board::walk_possible[7-d][2])
              &
              (
                ((me << board::walk_diff[d][3]) & board::walk_possible[7-d][3])
                |
                (
                  ((opp << board::walk_diff[d][3]) & board::walk_possible[7-d][3])
                  &
                  (
                    ((me << board::walk_diff[d][4]) & board::walk_possible[7-d][4])
                    |
                    (
                      ((opp << board::walk_diff[d][4]) & board::walk_possible[7-d][4])
                      &
                      (
                        ((me << board::walk_diff[d][5]) & board::walk_possible[7-d][5])
                        |
                        (
                          ((opp << board::walk_diff[d][5]) & board::walk_possible[7-d][5])
                          &
                          ((me << board::walk_diff[d][6]) & board::walk_possible[7-d][6])
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
  else if(count[0] < count[1]){ /* Opp wins */
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

void board::do_move_experimental(int move_id, std::bitset<64>* undo_data)
{
  assert(is_valid_move(move_id));
  
  undo_data->reset();
  
  const std::bitset<64> move_bit(1ul << move_id);
  
   
  for(int dir=4;dir<8;dir++){
    
    const int opp_dir = 7-dir;
    
    std::bitset<64> shifted_me[12];
    std::bitset<64> move_bit_dir[6];
    std::bitset<64> move_bit_opp_dir[6];
    std::bitset<64> shifted_opp_streak[10];
    
    const int* walk_diff_dir = walk_diff[dir];
    
    
    for(int i=0;i<6;i++){
      shifted_me[i] = (me << walk_diff_dir[i]);
      shifted_me[6+i] = (me >> walk_diff_dir[i]);
    }
    
    for(int i=0;i<6;i++){
      move_bit_dir[i] = (move_bit & walk_possible[dir][i+1]);
      move_bit_opp_dir[i] = (move_bit & walk_possible[opp_dir][i+1]);
    }
    
    shifted_opp_streak[0] = (opp << walk_diff_dir[0]);
    shifted_opp_streak[5] = (opp >> walk_diff_dir[0]);
    
    for(int i=1;i<5;i++){
      shifted_opp_streak[i] = 
        shifted_opp_streak[i-1] 
        & (opp << walk_diff_dir[i]);
      
      shifted_opp_streak[5+i] = 
        shifted_opp_streak[5+i-1] 
        & (opp >> walk_diff_dir[i]);
    }
    
    
    *undo_data |=
      opp
      &
      (
        ( // stepsize 1, 1st disc
          (move_bit_opp_dir[0] >> walk_diff[dir][0])
          & shifted_me[0]
        )
        |
        ( // stepsize 2, 1st disc
          (move_bit_opp_dir[1] >> walk_diff[dir][0])
          & shifted_opp_streak[0]
          & shifted_me[1]
        )
        |
        ( // stepsize 2, 2nd disc
          (move_bit_opp_dir[1] >> walk_diff[dir][1])
          & shifted_opp_streak[5]
          & shifted_me[0]
        )
        |
        ( // stepsize 3, 1st disc
          (move_bit_opp_dir[2] >> walk_diff[dir][0])
          & shifted_opp_streak[0]
          & shifted_me[2]
        )
        |
        ( // stepsize 3, 2nd disc
          (move_bit_opp_dir[2] >> walk_diff[dir][1])
          & shifted_opp_streak[5]
          & shifted_opp_streak[0]
          & shifted_me[1]
        )
        |
        ( // stepsize 3, 3rd disc
          (move_bit_opp_dir[2] >> walk_diff[dir][2])
           & shifted_opp_streak[5]
          & shifted_me[0]
        )
        |
        ( // stepsize 4, 1st disc
          (move_bit_opp_dir[3] >> walk_diff[dir][0])
          & shifted_opp_streak[2]
          & shifted_me[3]
        )
        |
        ( // stepsize 4, 2nd disc
          (move_bit_opp_dir[3] >> walk_diff[dir][1])
          & shifted_opp_streak[5]
          & shifted_opp_streak[1]
          & shifted_me[2]
        )
        |
        ( // stepsize 4, 3rd disc
          (move_bit_opp_dir[3] >> walk_diff[dir][2])
          & shifted_opp_streak[6]
          & shifted_opp_streak[0]
          & shifted_me[1]
        )
        |
        ( // stepsize 4, 4th disc
          (move_bit_opp_dir[3] >> walk_diff[dir][3])
          & shifted_opp_streak[7]
          & shifted_me[0]
        )
        |
        ( // stepsize 5, 1st disc
          (move_bit_opp_dir[4] >> walk_diff[dir][0])
          & shifted_opp_streak[3]
          & shifted_me[4]
        )
        |
        ( // stepsize 5, 2nd disc
          (move_bit_opp_dir[4] >> walk_diff[dir][1])
          & shifted_opp_streak[5]
          & shifted_opp_streak[2]
          & shifted_me[3]
        )
        |
        ( // stepsize 5, 3rd disc
          (move_bit_opp_dir[4] >> walk_diff[dir][2])
          & shifted_opp_streak[6]
          & shifted_opp_streak[1]
          & shifted_me[2]
        )
        |
        ( // stepsize 5, 4th disc
          (move_bit_opp_dir[4] >> walk_diff[dir][3])
          & shifted_opp_streak[7]
          & shifted_opp_streak[0]
          & shifted_me[1]
        )
        |
        ( // stepsize 5, 5th disc
          (move_bit_opp_dir[4] >> walk_diff[dir][4])
          & shifted_opp_streak[8]
          & shifted_me[0]
        )
        |
        ( // stepsize 6, 1st disc
          (move_bit_opp_dir[5] >> walk_diff[dir][0])
          & shifted_opp_streak[4]
          & shifted_me[5]
        )
        |
        ( // stepsize 6, 2nd disc
          (move_bit_opp_dir[5] >> walk_diff[dir][1])
          & shifted_opp_streak[5]
          & shifted_opp_streak[3]
          & shifted_me[4]
        )
        |
        ( // stepsize 6, 3rd disc
          (move_bit_opp_dir[5] >> walk_diff[dir][2])
          & shifted_opp_streak[6]
          & shifted_opp_streak[2]
          & shifted_me[3]
        )
        |
        ( // stepsize 6, 4th disc
          (move_bit_opp_dir[5] >> walk_diff[dir][3])
          & shifted_opp_streak[7]
          & shifted_opp_streak[1]
          & shifted_me[2]
        )
        |
        ( // stepsize 6, 5th disc
          (move_bit_opp_dir[5] >> walk_diff[dir][4])
          & shifted_opp_streak[8]
          & shifted_me[1]
        )
        |
        ( // stepsize 6, 6th disc
          (move_bit_opp_dir[5] >> walk_diff[dir][5])
          & shifted_opp_streak[9]
          & shifted_me[0]
        )
      );
      
    *undo_data |=
      opp
      &
      (
        ( // stepsize 1, 1st disc
          (move_bit_dir[0] << walk_diff[dir][0])
          & shifted_me[6]
        )
        |
        ( // stepsize 2, 1st disc
          (move_bit_dir[1] << walk_diff[dir][0])
          & shifted_opp_streak[5]
          & shifted_me[7]
        )
        |
        ( // stepsize 2, 2nd disc
          (move_bit_dir[1] << walk_diff[dir][1])
          & shifted_opp_streak[0]
          & shifted_me[6]
        )
        |
        ( // stepsize 3, 1st disc
          (move_bit_dir[2] << walk_diff[dir][0])
         & shifted_opp_streak[6]
          & shifted_me[8]
        )
        |
        ( // stepsize 3, 2nd disc
          (move_bit_dir[2] << walk_diff[dir][1])
          & shifted_opp_streak[0]
          & shifted_opp_streak[5]
          & shifted_me[7]
        )
        |
        ( // stepsize 3, 3rd disc
          (move_bit_dir[2] << walk_diff[dir][2])
          & shifted_opp_streak[1]
          & shifted_me[6]
        )
        |
        ( // stepsize 4, 1st disc
          (move_bit_dir[3] << walk_diff[dir][0])
          & shifted_opp_streak[7]
          & shifted_me[9]
        )
        |
        ( // stepsize 4, 2nd disc
          (move_bit_dir[3] << walk_diff[dir][1])
          & shifted_opp_streak[0]
         & shifted_opp_streak[6]
          & shifted_me[8]
        )
        |
        ( // stepsize 4, 3rd disc
          (move_bit_dir[3] << walk_diff[dir][2])
          & shifted_opp_streak[1]
          & shifted_opp_streak[5]
          & shifted_me[7]
        )
        |
        ( // stepsize 4, 4th disc
          (move_bit_dir[3] << walk_diff[dir][3])
          & shifted_opp_streak[2]
          & shifted_me[6]
        )
        |
        ( // stepsize 5, 1st disc
          (move_bit_dir[4] << walk_diff[dir][0])
          & shifted_opp_streak[8]
          & shifted_me[10]
        )
        |
        ( // stepsize 5, 2nd disc
          (move_bit_dir[4] << walk_diff[dir][1])
          & shifted_opp_streak[0]
          & shifted_opp_streak[7]
          & shifted_me[9]
        )
        |
        ( // stepsize 5, 3rd disc
          (move_bit_dir[4] << walk_diff[dir][2])
          & shifted_opp_streak[1]
          & shifted_opp_streak[6]
          & shifted_me[8]
        )
        |
        ( // stepsize 5, 4th disc
          (move_bit_dir[4] << walk_diff[dir][3])
          & shifted_opp_streak[2]
          & shifted_opp_streak[5]
          & shifted_me[7]
        )
        |
        ( // stepsize 5, 5th disc
          (move_bit_dir[4] << walk_diff[dir][4])
          & shifted_opp_streak[3]
          & shifted_me[6]
        )
        |
        ( // stepsize 6, 1st disc
          (move_bit_dir[5] << walk_diff[dir][0])
          & shifted_opp_streak[9]
          & shifted_me[11]
        )
        |
        ( // stepsize 6, 2nd disc
          (move_bit_dir[5] << walk_diff[dir][1])
          & shifted_opp_streak[0]
          & shifted_opp_streak[8]
          & shifted_me[10]
        )
        |
        ( // stepsize 6, 3rd disc
          (move_bit_dir[5] << walk_diff[dir][2])
          & shifted_opp_streak[1]
          & shifted_opp_streak[7]
          & shifted_me[9]
        )
        |
        ( // stepsize 6, 4th disc
          (move_bit_dir[5] << walk_diff[dir][3])
          & shifted_opp_streak[2]
          & shifted_opp_streak[6]
          & shifted_me[8]
        )
        |
        ( // stepsize 6, 5th disc
          (move_bit_dir[5] << walk_diff[dir][4])
          & shifted_opp_streak[3]
          & shifted_opp_streak[5]
          & shifted_me[7]
        )
        |
        ( // stepsize 6, 6th disc
          (move_bit_dir[5] << walk_diff[dir][5])
          & shifted_opp_streak[4]
          & shifted_me[6]
        )
      );
  }

  assert((me & (*undo_data)).none());
  assert((opp & (*undo_data)) == (*undo_data));
  assert((get_non_empty_fields() & bit[move_id]).none());
  
  me |= ((*undo_data) | bit[move_id]);
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

void board::check_do_move_experimental() const{
  assert((me & opp).none());
  
  std::bitset<64> moves,undo,undo_exp;
  
  get_valid_moves(&moves);
  board copy = *this;
  
  while(true){
    int move = find_first_set_64(moves.to_ulong());
    if(move == -1){
      break;
    }
    
    copy.do_move(move,&undo);
    copy = *this;
    copy.do_move_experimental(move,&undo_exp);
    copy = *this;
    
    
    if(undo != undo_exp){
      std::cout << "test failed:\n";
      show();
      std::cout << "move:\n";
      show_bitset(std::bitset<64>(1ul << move));
      
      std::cout << "working:\n";
      show_bitset(undo);
      std::cout << "experimental:\n";
      show_bitset(undo_exp);
    }
    else{
      std::cout << "test succeeded\n";
    }
    
    moves.reset(move);
  }
}
