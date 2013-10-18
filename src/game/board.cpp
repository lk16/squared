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

const int board::direction[8] = {
  -9,-8,-7,-1,1,7,8,9
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

void board::do_move(int field_id, board* out) const
{
  *out = *this;
  
  // if the square is not empty, it cannot be a valid move 
  if((get_non_empty_fields() & board::bit[field_id]).any()){
    CRASH;
  }
  
  
  // check if field_id is my color in any of the children
  board children[32];
  int move_count = get_children(children) - children;
  for(int i=0;i<move_count;i++){
    if((children[i].discs[turn] & board::bit[field_id]).any()){
      *out = children[i];
      return;
    }
  }
  CRASH;
}





board* board::get_children(board* out_begin) const
{
  board* out_end = out_begin;
  std::bitset<64> opp = discs[opponent(turn)];
  std::bitset<64> possible_moves;
  std::bitset<64> to_flip,tmp_mask;
  
  
  get_valid_moves(&possible_moves);
  
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
        if((std::bitset<64>(board::border[cur_field_id]) & board::bit[i]).any()){
          break;
        }
        
        // walk ahead        
        cur_field_id += board::direction[i]; 
        
        // current field = my color
        if((discs[turn] & board::bit[cur_field_id]).any()){
          to_flip |= tmp_mask;
          break;
        }
        
        // current field = opponent color
        if((discs[opponent(turn)] & board::bit[cur_field_id]).any()){
          tmp_mask.set(cur_field_id);
          continue;
        }
        
        // current fiend = empty
        break;
      }
    }
    
    assert(to_flip.any());
    if(to_flip.any()){
      if(out_end){
        *out_end = *this;
        out_end->discs[turn] |= to_flip | board::bit[field_id];
        out_end->discs[opponent(turn)] &= (~to_flip);
        out_end->switch_turn();
        ++out_end;
      }
    }  
  }
  return out_end;
}

void board::get_valid_moves(std::bitset<64>* out) const
{
  out->reset();
  
  const std::bitset<64>& opp = discs[opponent(turn)].to_ulong();
  const std::bitset<64>& mine = discs[turn].to_ulong();
  
  for(int d=0;d<4;d++){
    
    int diff = -board::direction[d];
    assert(diff > 0);
    
    *out |= 
    (
      ((opp << (diff*1)) & board::walk_possible[d][0]) 
      & 
      (
        ((mine << (diff*2)) & board::walk_possible[d][1])
        |
        (
          ((opp << (diff*2)) & board::walk_possible[d][1])
          &
          (
            ((mine << (diff*3)) & board::walk_possible[d][2])
            |
            (
              ((opp << (diff*3)) & board::walk_possible[d][2])
              &
              (
                ((mine << (diff*4)) & board::walk_possible[d][3])
                |
                (
                  ((opp << (diff*4)) & board::walk_possible[d][3])
                  &
                  (
                    ((mine << (diff*5)) & board::walk_possible[d][4])
                    |
                    (
                      ((opp << (diff*5)) & board::walk_possible[d][4])
                      &
                      (
                        ((mine << (diff*6)) & board::walk_possible[d][5])
                        |
                        (
                          ((opp << (diff*6)) & board::walk_possible[d][5])
                          &
                          ((mine << (diff*7)) & board::walk_possible[d][6])
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
  
    for(int d=0;d<4;d++){
    
    int diff = -board::direction[d];
    assert(diff > 0);
    
    *out |= 
    (
      ((opp << (diff*1)) & board::walk_possible[d][0]) 
      & 
      (
        ((mine << (diff*2)) & board::walk_possible[d][1])
        |
        (
          ((opp << (diff*2)) & board::walk_possible[d][1])
          &
          (
            ((mine << (diff*3)) & board::walk_possible[d][2])
            |
            (
              ((opp << (diff*3)) & board::walk_possible[d][2])
              &
              (
                ((mine << (diff*4)) & board::walk_possible[d][3])
                |
                (
                  ((opp << (diff*4)) & board::walk_possible[d][3])
                  &
                  (
                    ((mine << (diff*5)) & board::walk_possible[d][4])
                    |
                    (
                      ((opp << (diff*5)) & board::walk_possible[d][4])
                      &
                      (
                        ((mine << (diff*6)) & board::walk_possible[d][5])
                        |
                        (
                          ((opp << (diff*6)) & board::walk_possible[d][5])
                          &
                          ((mine << (diff*7)) & board::walk_possible[d][6])
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
  
  for(int d=4;d<8;d++){
    
    int diff = board::direction[d];
    assert(diff > 0);
    
    *out |= 
    (
      ((opp >> (diff*1)) & board::walk_possible[d][0]) 
      & 
      (
        ((mine >> (diff*2)) & board::walk_possible[d][1])
        |
        (
          ((opp >> (diff*2)) & board::walk_possible[d][1])
          &
          (
            ((mine >> (diff*3)) & board::walk_possible[d][2])
            |
            (
              ((opp >> (diff*3)) & board::walk_possible[d][2])
              &
              (
                ((mine >> (diff*4)) & board::walk_possible[d][3])
                |
                (
                  ((opp >> (diff*4)) & board::walk_possible[d][3])
                  &
                  (
                    ((mine >> (diff*5)) & board::walk_possible[d][4])
                    |
                    (
                      ((opp >> (diff*5)) & board::walk_possible[d][4])
                      &
                      (
                        ((mine >> (diff*6)) & board::walk_possible[d][5])
                        |
                        (
                          ((opp >> (diff*6)) & board::walk_possible[d][5])
                          &
                          ((mine >> (diff*7)) & board::walk_possible[d][6])
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
  if((get_non_empty_fields() & board::bit[field_id]).any()){
    return false;
  }
  
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
      cur_field_id += board::direction[i]; 
      
      // current field = my color
      if((discs[turn] & board::bit[cur_field_id]).any()){
        (*undo_data) |= tmp_mask;
        break;
      }
      
      // current field = opponent color
      if((discs[opponent(turn)] & board::bit[cur_field_id]).any()){
        tmp_mask |= board::bit[cur_field_id];
        continue;
      }
      
      // current fiend = empty
      break;
    }
  }
  if(undo_data->none()){
    return false;
  }
  
  
  assert((discs[turn] & (*undo_data)).none());
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
  discs[turn] &= ~(board::bit[field_id]);
  discs[opponent(turn)] |= (*undo_data);  
  
  
  assert((discs[turn] & (*undo_data)).none());
  assert((discs[opponent(turn)] & (*undo_data)) == (*undo_data));
  assert((discs[WHITE] | discs[BLACK]).test(field_id) == false);
}

std::set<board> board::get_descendants(int depth) const
{
  std::set<board> a,b;
  board buff[32];
  
  
  a.insert(*this);
  
  if(depth == 0){
    return a;
  }
  
  std::set<board>::const_iterator it;
  
  for(int d=0;d<depth;d++){
    b.clear();
    for(it=a.begin();it!=a.end();it++){
      const board* buff_end = it->get_children(buff);
      for(const board* buff_it=buff;buff_it!=buff_end;buff_it++){
        b.insert(*buff_it);
      }
    }
    b.swap(a);
    if(a.empty()){
      break; // we cannot expand an empty depth level
    }
  }
  
  return b;
}





int board::get_stable_disc_count_diff(int max_depth) const
{
  board stable;
  stable.set_all();
  
  std::set<board> desc = get_descendants(max_depth);
  
  for(std::set<board>::const_iterator it=desc.begin();it!=desc.end();it++){
    stable &= *it;
  }
  
  return stable.discs[WHITE].count() - stable.discs[BLACK].count();
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
