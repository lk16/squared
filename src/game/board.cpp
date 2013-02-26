#include "board.hpp"

bool board::do_move(int field_id,board* result) const
{
  
  int x,y,dx,dy,curx,cury;
  *result = *this;
  std::bitset<TOTAL_FIELDS> mask,tmp_mask;
  
  mask.reset();
  tmp_mask.reset();
  
  if(discs[BLACK].test(field_id) || discs[WHITE].test(field_id)){
    return false;
  }
  
  x = field_id%FIELD_SIZE;
  y = field_id/FIELD_SIZE;
  
  
  for(dx=-1;dx<=1;dx++){
    for(dy=-1;dy<=1;dy++){
      if(!valid_coord(x+2*dx) || !valid_coord(y+2*dy) || (dx==0 && dy==0)){
        continue;
      }
      tmp_mask.reset();
      curx = x;
      cury = y;
      while(1){
        curx += dx;
        cury += dy;
        if(!valid_coord(curx) || !valid_coord(cury) || result->has_color(cury*FIELD_SIZE+curx,EMPTY)){
          break;
        }
        if(result->has_color(cury*FIELD_SIZE+curx,opponent(turn))){
          tmp_mask.set(FIELD_SIZE*cury + curx);
          continue;
        }
        if(result->has_color(cury*FIELD_SIZE+curx,turn)){
          if(tmp_mask.any()){
            mask |= tmp_mask;
          }
          break;
        }
      }
    }
  }
  
  if(mask.any()){
    mask.set(field_id);
    result->discs[turn] |= mask;
    result->discs[opponent(turn)] &= (~mask);
    
    result->turn = opponent(result->turn);
    return true;
  }
  return false;
}

void board::get_children(board* array,int* move_count) const
{
  int field_id,i;
  
  i=0;
  for(field_id=0;field_id<TOTAL_FIELDS;field_id++){
    if(do_move(field_id,array+i)){
      i++;
    }
  }
  *move_count = i;
}

int board::count_moves() const
{
  int res,field_id;
  board dummy;
  res=0;
  for(field_id=0;field_id<TOTAL_FIELDS;field_id++){
    if(do_move(field_id,&dummy)){
        res++;
    }
  }
  return res;
}


bool board::has_moves() const
{
  int field_id;
  board dummy;
  for(field_id=0;field_id<TOTAL_FIELDS;field_id++){
    if(do_move(field_id,&dummy)){
      return true;
    }
  }
  return false;
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
      switch(get_color(y*FIELD_SIZE+x)){
        case BLACK: 
          std::cout << "\033[31;1m@\033[0m";
          break;
        case WHITE:
          std::cout << "\033[34;1m@\033[0m";
          break;
        case EMPTY: 
          if(do_move(y*FIELD_SIZE+x,&dummy)){
            std::cout << '.';
            break;
          }  
          else{
            std::cout << ' ';
            break;
          }
      }
      std::cout << ' ';
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

int board::count_flipped(int field_id, color c) const
{
  
  int x,y,dx,dy,dist,curx,cury;
  int res;
  
  res = 0;
  
  if(!has_color(field_id,EMPTY)){
    return 0;
  }
  
  x = field_id%FIELD_SIZE;
  y = field_id/FIELD_SIZE;
  
  
  for(dx=-1;dx<=1;dx++){
    for(dy=-1;dy<=1;dy++){
      if(!valid_coord(x+2*dx) || !valid_coord(y+2*dy)){
        continue;
      }
      if(dx!=0 || dy!=0){
        dist = -1;
        curx = x;
        cury = y;
        while(1){
          dist++;
          curx += dx;
          cury += dy;
          if(!valid_coord(curx) || !valid_coord(cury) || has_color(cury*FIELD_SIZE+curx,EMPTY)){
            break;
          }
          if(has_color(valid_coord(cury*FIELD_SIZE+curx),opponent(c))){
            continue;
          }
          if(has_color(valid_coord(cury*FIELD_SIZE+curx),c)){
            // this is possible because we initialized dist at -1
            res += dist;
            break;
          }
        }
      }
    }
  }
  return res;
}

int board::get_mobility(color c) const
{
  int field_id,res;
  res = 0;
  for(field_id=0;field_id<TOTAL_FIELDS;field_id++){
    res += count_flipped(field_id,c);
  }
  return res;
}

int board::get_disc_diff() const
{
  int count[2];
  
  count[BLACK] = count_discs(BLACK);
  count[WHITE] = count_discs(WHITE);
  
  if(count[BLACK] > count[WHITE]){ /* black wins */
    return ((-TOTAL_FIELDS)+(2*count[WHITE]));
  }
  else if(count[WHITE] > count[BLACK]){ /* white wins */
    return (TOTAL_FIELDS-(2*count[BLACK]));
  }
  else{ /* draw */
    return 0;
  }
}

void board::oneliner(char* out) const
{  
  char* cur = out;
  int i;
  for(i=0;i<TOTAL_FIELDS;i++){
    switch(get_color(i)){
      case WHITE:
        *cur = 'o';
        break;
      case BLACK:
        *cur = 'x';
        break;
      case EMPTY:
        *cur = '.';
        break;
    }
    cur++;
  }
  *cur = '\0';
}

board::board(const char* in)
{
  const char* cur = in;
  int i;
  for(i=0;i<TOTAL_FIELDS;i++){
    switch(*cur){
      case WHITE:
        discs[WHITE].set(i);
        break;
      case BLACK:
        discs[BLACK].set(i);
        break;
      case EMPTY:
        break;
    }
    cur++;
  }
}

