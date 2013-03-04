#include "board.hpp"

int board::count_moves(color c) const
{
  int res;
  int x,y;
  
  res=0;
  for(y=0;y<FIELD_SIZE;y++){
    for(x=0;x<FIELD_SIZE;x++){
      if(is_valid_move(x,y,c)){
        res++;
      }
    }
  }
  return res;
}

board board::do_move(int x, int y) const
{
  int dx,dy,dist,curx,cury;
  board result(*this);
  std::bitset<TOTAL_FIELDS> mask;
    
  for(dx=-1;dx<=1;dx++){
    for(dy=-1;dy<=1;dy++){
      if(!on_board(x+2*dx,y+2*dy) || (dx==0 && dy==0)){
        continue;
      }
      mask = 0ul;
      dist = 0;
      curx = x;
      cury = y;
      while(1){
        dist++;
        curx += dx;
        cury += dy;
        if(!on_board(curx,cury) || result.has_color(curx,cury,EMPTY)){
          break;
        }
        if(result.has_color(curx,cury,opponent(turn))){
          mask.set(FIELD_SIZE*cury + curx);
          continue;
        }
        if(result.has_color(curx,cury,turn)){
          if(dist>=2){
            result.discs[turn] |= mask;
            result.discs[opponent(turn)] &= (~mask);
          }
          break;
        }
      }
    }
  }
  result.turn = opponent(result.turn);
  result.discs[turn].set(FIELD_SIZE*y + x);
  return result;
}

bool board::is_valid_move(int x, int y) const
{
  return is_valid_move(x,y,turn);
}


bool board::is_valid_move(int x, int y, color c) const
{
  int dx,dy,dist,curx,cury;
  
  if(!has_color(x,y,EMPTY)){
    return false;
  }
  for(dx=-1;dx<=1;dx++){
    for(dy=-1;dy<=1;dy++){
      if(!on_board(x+2*dx,y+2*dy)){
        continue;
      }
      if(dx!=0 || dy!=0){
        dist = 0;
        while(1){
          dist++;
          curx = x+dx*dist;
          cury = y+dy*dist;
          if(!on_board(curx,cury) || has_color(curx,cury,EMPTY)){
            break;
          }
          if(has_color(curx,cury,opponent(c))){
            continue;
          }
          if(has_color(curx,cury,c)){
            if(dist>=2){
              return true;
            }
            break;
          }
        }
      }
    }
  }
  return false;
}

void board::show() const
{
  int x,y;
  
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
          if(is_valid_move(x,y,turn)){
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

void board::get_children(board* array,int* move_count) const
{
  int x,y,i;
 
  i=0;
  for(y=0;y<FIELD_SIZE;y++){
    for(x=0;x<FIELD_SIZE;x++){
      if(is_valid_move(x,y,turn)){
        array[i] = do_move(x,y);
        i++;
      }
    }
  }
  *move_count = i;
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

int board::mobility(color c) const
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



