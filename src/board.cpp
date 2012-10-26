#include "board.hpp"

int board::count_moves(color c) const
{
  int res;
  int x,y;
  
  res=0;
  for(y=0;y<8;y++){
    for(x=0;x<8;x++){
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
  unsigned long mask;
    
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
          mask |= (1ul << (8*cury + curx));
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
  result.discs[turn] |= (1ul << (8*y + x));
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
#ifndef NDEBUG
  int x,y;
  
  std::cout << "+-----------------+\n";
  for(y=0;y<8;y++){
    std::cout << "| ";
    for(x=0;x<8;x++){
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
  std::cout << "+-----------------+\n";
#endif
}

std::list<board> board::get_children() const
{
  int x,y;
  std::list<board> res;
  
  for(y=0;y<8;y++){
    for(x=0;x<8;x++){
      if(is_valid_move(x,y,turn)){
        res.push_back(do_move(x,y));
      }
    }
  }
  return res;
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
  for(x=0;x<8;x++){
    for(y=0;y<8;y++){
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
    return ((-64)+(2*count[WHITE]));
  }
  else if(count[BLACK] < count[WHITE]){ /* white wins */
    return (64-(2*count[BLACK]));
  }
  else{ /* draw */
    return 0;
  }
}



