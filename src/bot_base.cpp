#include "bot_base.hpp"

bot_base::bot_base(color _c, int _max_depth, int _max_endgame_depth):
  c(_c),
  max_depth(_max_depth),
  max_endgame_depth(_max_endgame_depth),
  look_ahead(5)
{
}

board* bot_base::do_move(const board* b) 
{
  int depth_limit,time_diff;
  unsigned int id,best_move_id;
  board *res;
  std::list<board_with_id> ahead;
  std::list<board> children;
  std::list<board>::const_iterator it;
  
  nodes = 0;
  start_time = std::time(NULL);
  
  
  depth_limit = (b->max_moves_left() <= max_endgame_depth) ? b->max_moves_left() : max_depth;
  children = b->get_children();
  assert(children.size()>0);
  
  best_move_id = -1;
  res = NULL;
  
  if(children.size()==1){
    return new board(children.front());
  } 
  
  
  id = 0;
  if(depth_limit - look_ahead < 3){
   /* if lookahead is not useful (enough) dont do it */
    for(it=children.begin();it!=children.end();it++){
      ahead.push_back(board_with_id(new board(*it),id));
      id++;
    }
    best_move_id = alpha_beta(ahead,depth_limit);
  }
  else{
    /* do lookahead search */
    for(it=children.begin();it!=children.end();it++){
      add_moves_ahead(ahead,id,&*it,look_ahead-1);
      id++;
    }
    
    /* sort found moves in ascending (heuristic) order, O(n log n) complexity */
    ahead.sort();
       

    /* negamax_internal_loop deletes all its pointers */
    assert(depth_limit-look_ahead > 0);
    best_move_id = alpha_beta(ahead,depth_limit-look_ahead);
  }
 
 
  /* delete all non-optimal moves from memory */
  id = 0;
  SHOW_VAR(children.size());
  while(!children.empty()){
    if(id==best_move_id){ 
      res = new board(children.front());
      break;
    }
    children.pop_front();
    id++;
  }
  
  time_diff = (std::time(NULL)-start_time);
  std::cout << nodes << " nodes in " << time_diff << " seconds: ";
  std::cout << nodes/(time_diff==0 ? 1 : time_diff) << " nodes / sec\n";
  
  assert(res);
  return res;
}


int bot_base::alpha_beta(std::list<board_with_id>& list, int depth_remaining)
{
  int best_move_id,beta,best_heur;
  
  best_move_id = -1;
  beta = 9999;
  
  while(!list.empty()){
    
    // TODO parallelize this v
    beta = alpha_beta_recursive(list.front().b,-9999,beta,depth_remaining-1);

    SHOW_VAR(beta);
    if(beta < best_heur){
      best_heur = beta;
      best_move_id = list.front().id;
    }
    list.pop_front();
  }
  return best_move_id;
}


int bot_base::alpha_beta_recursive(const board* b,int alpha, int beta,int depth_remaining)
{
  std::list<board> children;
  board tmp;
  
  nodes++;
  
  if(b->test_game_ended()){ 
    return (c==WHITE ? 1 : -1) * 100 * b->get_disc_diff();
  }
  if(depth_remaining==0){
    return (c==WHITE ? 1 : -1) * heuristic(b);
  }
  
  children = b->get_children();
  if(children.empty()){
    tmp = board(*b);
    tmp.turn = opponent(b->turn);
    return alpha_beta_recursive(&tmp,alpha,beta,depth_remaining-1);
  }
  
  if(b->turn == c){
    while(!children.empty()){
      alpha = max(alpha,alpha_beta_recursive(&children.front(),alpha,beta,depth_remaining-1));
      if(alpha>=beta){
        break;
      }
      children.pop_front();      
    }
    return alpha; 
  }
  else{
    while(!children.empty()){
      beta = min(beta,alpha_beta_recursive(&children.front(),alpha,beta,depth_remaining-1));
      if(alpha>=beta){
        break;
      }
      children.pop_front();
    }
    return beta;
  }
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
  board next;
    
  nodes++;
  
  if(moves_remaining == 0){
    assert(b->turn != EMPTY);
    vec.push_back(board_with_id(b,id));
    return;
  }
  
  for(x=0;x<8;x++){
    for(y=0;y<8;y++){
      if(b->is_valid_move(x,y,c)){
        next = b->do_move(x,y);
        add_moves_ahead(vec,id,&next,moves_remaining-1);
      }
    }
  }
}
  
  
  
