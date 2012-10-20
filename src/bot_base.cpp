#include "bot_base.hpp"

bot_base::bot_base(color _c, int _max_depth, int _max_endgame_depth):
  max_depth(_max_depth),
  max_endgame_depth(_max_endgame_depth),
  look_ahead(5),
  c(_c)  
{
}

board* bot_base::do_move(const board* b) 
{
  int depth_limit,time_diff,best_heur,tmp_heur;
  unsigned int id,best_move_id;
  board *res;
  std::list<board> children;
  std::list<board>::iterator it;
  
  nodes = 0;
  start_time = std::time(NULL);
  best_heur = -9999;
  
  
  depth_limit = (b->max_moves_left() <= max_endgame_depth) ? b->max_moves_left() : max_depth;
  children = b->get_children();
  assert(children.size()>0);
  
  best_move_id = -1;
  res = NULL;
  
  if(children.size()==1){
    return new board(children.front());
  } 
  
  id = 0;
  it = children.begin();
  
  while(it!=children.end()){
    tmp_heur = alpha_beta(&*it,best_heur,6400,depth_limit);
    if(tmp_heur > best_heur){
      best_heur = tmp_heur;
      best_move_id = id;
      res = new board(*it);
    }
    it++;
    children.pop_front();
    id++;
  }

  time_diff = (std::time(NULL)-start_time);
  std::cout << nodes << " nodes in " << time_diff << " seconds: ";
  std::cout << nodes/(time_diff==0 ? 1 : time_diff) << " nodes / sec\n";
  
  assert(res);
  return res;
}




int bot_base::alpha_beta(const board* b,int alpha, int beta,int depth_remaining)
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
    return alpha_beta(&tmp,alpha,beta,depth_remaining-1);
  }
  
  if(b->turn == c){
    while(!children.empty()){
      alpha = max(alpha,alpha_beta(&children.front(),alpha,beta,depth_remaining-1));
      if(alpha>=beta){
        break;
      }
      children.pop_front();      
    }
    return alpha; 
  }
  else{
    while(!children.empty()){
      beta = min(beta,alpha_beta(&children.front(),alpha,beta,depth_remaining-1));
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

  
  
