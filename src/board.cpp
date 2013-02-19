#include "board.hpp"

bool board::do_move(int x, int y,board* result) const
{
  int dx,dy,curx,cury;
  *result = *this;
  std::bitset<TOTAL_FIELDS> mask,tmp_mask;
  
  mask.reset();
  tmp_mask.reset();
  
  if(discs[BLACK].test(y*FIELD_SIZE+x) || discs[WHITE].test(y*FIELD_SIZE+x)){
    return false;
  }
  
  for(dx=-1;dx<=1;dx++){
    for(dy=-1;dy<=1;dy++){
      if(!on_board(x+2*dx,y+2*dy) || (dx==0 && dy==0)){
        continue;
      }
      tmp_mask.reset();
      curx = x;
      cury = y;
      while(1){
        curx += dx;
        cury += dy;
        if(!on_board(curx,cury) || result->has_color(curx,cury,EMPTY)){
          break;
        }
        if(result->has_color(curx,cury,opponent(turn))){
          tmp_mask.set(FIELD_SIZE*cury + curx);
          continue;
        }
        if(result->has_color(curx,cury,turn)){
          if(tmp_mask.any()){
            mask |= tmp_mask;
          }
          break;
        }
      }
    }
  }
  
  if(mask.any()){
    mask.set(FIELD_SIZE*y + x);
    result->discs[turn] |= mask;
    result->discs[opponent(turn)] &= (~mask);
    
    result->turn = opponent(result->turn);
    return true;
  }
  return false;
}

void board::get_children(board* array,int* move_count) const
{
  int x,y,i;
  
  i=0;
  for(y=0;y<FIELD_SIZE;y++){
    for(x=0;x<FIELD_SIZE;x++){
      if(do_move(x,y,array+i)){
        i++;
      }
    }
  }
  *move_count = i;
}

int board::count_moves() const
{
  int res,x,y;
  board dummy;
  res=0;
  for(y=0;y<FIELD_SIZE;y++){
    for(x=0;x<FIELD_SIZE;x++){
      if(do_move(x,y,&dummy)){
        res++;
      }
    }
  }
  return res;
}


bool board::has_moves() const
{
  int x,y;
  board dummy;
  for(y=0;y<FIELD_SIZE;y++){
    for(x=0;x<FIELD_SIZE;x++){
      if(do_move(x,y,&dummy)){
        return true;
      }
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
      switch(get_color(x,y)){
        case BLACK: 
          std::cout << "\033[31;1m@\033[0m";
          break;
        case WHITE:
          std::cout << "\033[34;1m@\033[0m";
          break;
        case EMPTY: 
          if(do_move(x,y,&dummy)){
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

int board::count_flipped(int x, int y, color c) const
{
  
  int dx,dy,dist,curx,cury;
  int res;
  
  res = 0;
  
  if(!has_color(x,y,EMPTY)){
    return 0;
  }
  for(dx=-1;dx<=1;dx++){
    for(dy=-1;dy<=1;dy++){
      if(!on_board(x+2*dx,y+2*dy)){
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
          if(!on_board(curx,cury) || has_color(curx,cury,EMPTY)){
            break;
          }
          if(has_color(curx,cury,opponent(c))){
            continue;
          }
          if(has_color(curx,cury,c)){
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
  int x,y,res;
  res = 0;
  for(x=0;x<FIELD_SIZE;x++){
    for(y=0;y<FIELD_SIZE;y++){
      res += count_flipped(x,y,c);
    }
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
    switch(get_color(i/FIELD_SIZE,i%FIELD_SIZE)){
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

