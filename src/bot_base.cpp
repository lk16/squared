#include "bot_base.hpp"

bot_base::bot_base(color c, int max_depth, int max_endgame_depth):
  c(c),
  max_depth(max_depth),
  max_endgame_depth(max_endgame_depth),
  look_ahead(5)
{
}

board* bot_base::do_move(const board* b) 
{
  int depth_limit;
  unsigned int id,best_move_id;
  board *res;
  std::list<board_with_id> ahead;
  std::list<board*> children;
  std::list<board*>::const_iterator it;
  
  depth_limit = (b->max_moves_left() <= max_endgame_depth) ? b->max_moves_left() : max_depth;
  children = b->get_children();
  assert(children.size()>0);
  
  best_move_id = -1;
  res = NULL;
  
  if(children.size()==1){
    return children.front();
  }
  
  
  
  id = 0;
  if(depth_limit - look_ahead < 2){
    /* if lookahead is not useful (enough) dont do it */
    for(it=children.begin();it!=children.end();it++){
      ahead.push_back(board_with_id(new board(**it),id));
      id++;
    }
    
    /* negamax_internal_loop deletes all its pointers */
    best_move_id = negamax_internal_loop(ahead,depth_limit-1);
  }
  else{
    /* do lookahead search */
    for(it=children.begin();it!=children.end();it++){
      add_moves_ahead(ahead,id,*it,look_ahead-1);
      id++;
    }
    
    /* sort found moves in ascending (heuristic) order, O(n log n) complexity */
    ahead.sort();

    /* negamax_internal_loop deletes all its pointers */
    best_move_id = negamax_internal_loop(ahead,depth_limit-look_ahead);
  }
 
  /* delete all non-optimal moves from memory */
  id = 0;
  while(!children.empty()){
    if(id==best_move_id){
      res = new board(*children.front());
    }
    delete children.front();
    children.pop_front();
    id++;
  }
  
  assert(res);
  return res;
}


int bot_base::negamax_internal_loop(std::list<board_with_id>& list, int moves_remaining)
{
  int tmp_heur,best_heur,best_move_id;
  
  best_heur = -9999;
  best_move_id = -1;
  
  while(!list.empty()){
    
    // TODO parallelize this v
    tmp_heur = negamax(list.back().b,best_heur,9999,moves_remaining);
    if(tmp_heur > best_heur){
      best_heur = tmp_heur;
      best_move_id = list.back().id;
    }
    delete list.back().b;
    list.pop_back();
  }
  
  return best_move_id;
}


int bot_base::negamax(const board* b,int alpha, int beta, int depth_remaining)
{
  int x,y,val;
  bool move_found;
  board *next,tmp;
  
  
  
  if(depth_remaining==0 || b->test_game_ended()){
    if(b->count_moves(WHITE)==0 && b->count_moves(BLACK)==0){
      return 100*(b->count_discs(WHITE)-b->count_discs(BLACK));
    }
    return (b->turn==WHITE ? 1 : -1) * heuristic(b);
  }
  
  move_found = false;
  
  for(y=0;y<8;y++){
    for(x=0;x<8;x++){
      if(b->is_valid_move(x,y,b->turn)){
        next = b->do_move(x,y);
        move_found = true;
        val = -negamax(next,-beta,-alpha,depth_remaining-1);
        if(val >= beta){
          return val;
        }
        if(val >= alpha){
          alpha = val;
        }
        delete next;
      }
    }
  }
  if(!move_found){
    assert(b->has_moves(opponent(b->turn)));
    tmp = board(*b);
    tmp.turn = opponent(b->turn);
    return -negamax(&tmp,-beta,-alpha,depth_remaining);
  }
  return alpha; 
}

int bot_base::heuristic(const board* b)
{
  b = NULL; /* prevent compiler complaints */
  std::cout << "This is bot_base::heuristic(). Don't call me! Call my superclass instead! \n";
  assert(false);
  return 0;
}

void bot_base::add_moves_ahead(std::list<board_with_id>& vec,unsigned id,
                               const board* b,int moves_remaining)
{
  int x,y;
  board* next;
    
  if(moves_remaining == 0){
    vec.push_back(board_with_id(b,id));
  }
  
  for(x=0;x<8;x++){
    for(y=0;y<8;y++){
      if(b->is_valid_move(x,y,c)){
        next = b->do_move(x,y);
          add_moves_ahead(vec,id,next,moves_remaining-1);
          delete next;
      }
    }
  }
}
  
  
  
