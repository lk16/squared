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

board* board::do_move(int x, int y) const
{
  int dx,dy,dist,curx,cury,ax,ay;
  board *result;
  
  result = new board(*this);
  
  for(dx=-1;dx<=1;dx++){
    for(dy=-1;dy<=1;dy++){
      if(dx!=0 || dy!=0){
        dist = 0;
        curx = x;
        cury = y;
        while(1){
          dist++;
          curx += dx;
          cury += dy;
          if(!on_board(curx,cury) || result->has_color(curx,cury,EMPTY)){
            break;
          }
          if(result->has_color(curx,cury,opponent(turn))){
            continue;
          }
          if(result->has_color(curx,cury,turn)){
            if(dist>=2){
              ax = x;
              ay = y;
              while(!(ax==curx && ay==cury)){
                result->set_color(ax,ay,turn);
                ax+=dx;
                ay+=dy;
              }
              result->set_color(x,y,turn);
            }
            break;
          }
        }
      }
    }
  }
  result->turn = opponent(turn);
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
  return 0;
}

void board::show(color c, bool mark_moves) const
{
  int x,y;
  char mark;
  
  mark = 'a';
  std::cout << "+-----------------+\n";
  for(y=0;y<8;y++){
    std::cout << "| ";
    for(x=0;x<8;x++){
      switch(get_color(x,y)){
        case BLACK: 
          std::cout << "@";
          break;
        case WHITE:
          std::cout << "O";
          break;
        case EMPTY: 
          if(is_valid_move(x,y,c)){
            if(mark_moves){
              std::cout << mark; 
              mark++;
              break;              
            }
            else{
              std::cout << '.';
              break;
            }
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
}

std::list<board*> board::get_children() const
{
  int x,y;
  std::list<board*> res;
  
  /* find out all possible moves */
  for(y=0;y<8;y++){
    for(x=0;x<8;x++){
      if(is_valid_move(x,y,turn)){
        res.push_back(do_move(x,y));
      }
    }
  }
  return res;
}

int board::get_flipped(int x, int y, color c) const
{
  int dx,dy,dist,curx,cury,ax,ay;
  int res;
  
  res = 0;
  
  if(has_color(x,y,EMPTY)){
    return 0;
  }
  for(dx=-1;dx<=1;dx++){
    for(dy=-1;dy<=1;dy++){
      if(dx!=0 || dy!=0){
        dist = 0;
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
            if(dist>=2){
             
              ax = x;
              ay = y;
              res += max(abs(curx-x),abs(cury-y));
            }
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
      res = get_flipped(x,y,c);
    }
  }
  return res;
}

