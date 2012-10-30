#include "bot_base.hpp"

bot_base::bot_base(color _c, int _max_depth, int _max_endgame_depth):
  max_depth(_max_depth),
  max_endgame_depth(_max_endgame_depth),
  look_ahead(2),
  c(_c),
  prev_move_time(std::time(NULL))
{
}

board* bot_base::do_move(const board* b) 
{
  int depth_limit,time_diff,best_heur,tmp_heur,move_count;
  unsigned int id,best_move_id;
  board *res,children[32];
  std::string text;
  
  nodes = 0;
  prev_move_time = std::time(NULL);
  best_heur = -6400;
  
  
  depth_limit = (b->max_moves_left() <= max_endgame_depth) ? 60 : max_depth;
  b->get_children(children,&move_count);
  assert(move_count>0);
  
  best_move_id = -1;
  res = NULL;
  
  if(move_count==1){
    return new board(children[0]);
  } 
  
  /*if(depth_limit - look_ahead > 3){
    sort_boards(children,look_ahead);
  }*/
  
  
  for(id=0;id<move_count;id++){
    tmp_heur = alpha_beta(children + id,best_heur,6400,depth_limit);
    if(tmp_heur > best_heur){
      best_heur = tmp_heur;
      text = "move " + tostr<int>(id+1) + "/" + tostr<int>(move_count) +": heuristic == ";
      text += tostr<int>(best_heur) + "\n";
      std::cout << text;
      /* TODO update statusbar msg */
      
      best_move_id = id;
      res = new board(children[id]);
    }
    else{
      text = "move " + tostr<int>(id+1) + "/" + tostr<int>(move_count) +": heuristic <= ";
      text += tostr<int>(best_heur) + "\n";
      std::cout << text;
      /* TODO update statusbar msg */
    }
  }

  time_diff = (std::time(NULL)-prev_move_time);
  std::cout << nodes << " nodes in " << time_diff << " seconds: ";
  std::cout << nodes/(time_diff==0 ? 1 : time_diff) << " nodes / sec\n";
  
  assert(res);
  return res;
}




int bot_base::alpha_beta(const board* b,int alpha, int beta,int depth_remaining)
{
  board children[32];
  int move_count,id;
  board tmp;
  
  nodes++;
  
  if(b->test_game_ended()){
    return (c==WHITE ? 1 : -1) * 100 * b->get_disc_diff();
  }
  if(depth_remaining==0){
    return heuristic(b);
  }
  
  b->get_children(children,&move_count);
  if(move_count==0){
    tmp = board(*b);
    tmp.turn = opponent(b->turn);
    return alpha_beta(&tmp,alpha,beta,depth_remaining/*-1*/);
  }
  
  if(b->turn == c){
    for(id=0;id<move_count;id++){
      alpha = max(alpha,alpha_beta(children + id,alpha,beta,depth_remaining-1));
      if(alpha>=beta){
        break;
      }
    }
    return alpha; 
  }
  else{
    for(id=0;id<move_count;id++){
      beta = min(beta,alpha_beta(children + id,alpha,beta,depth_remaining-1));
      if(alpha>=beta){
        break;
      }
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

void bot_base::sort_boards(std::list<board>& vec, int depth_limit)
{
  std::vector<int> heur(vec.size(),0);
  std::list<board>::iterator it;
  bool loop;
  int id = 0;
  int best_heur = -6400;
  
  for(it=vec.begin();it!=vec.end();it++){
    heur[id] = alpha_beta(&*it,best_heur,6400,depth_limit);
    if(heur[id] > best_heur){
      best_heur = heur[id];
    }
  }
  
  id = 0;
  it = vec.begin();
  do{
    loop = false;
    for(id=0;id<vec.size()-1;id++){
      if(heur[id] > heur[id+1]){
        std::swap(heur[id],heur[id+1]);
        std::swap(*it,*(it++));
        assert(it != vec.begin());
        loop = true;
      }
    }
  }while(loop);
  
  
  
  
}
  
  
