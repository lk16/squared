#include "bot_base.hpp"

bot_base::bot_base(color _c, int _max_depth, int _max_endgame_depth):
  max_depth(_max_depth),
  max_endgame_depth(_max_endgame_depth),
  look_ahead(6),
  c(_c),
  prev_move_time(std::time(NULL))
{
}

void bot_base::do_move(const board* b,board* res) 
{
  int depth_limit,time_diff,best_heur,tmp_heur,move_count;
  unsigned int id,best_move_id;
  board moves[TOTAL_FIELDS/2];
  std::string text;
  
  nodes = 0;
  prev_move_time = std::time(NULL);
  best_heur = MIN_HEURISTIC;
  
  
  depth_limit = (b->max_moves_left() <= max_endgame_depth) ? (TOTAL_FIELDS-4) : max_depth;
  b->get_children(moves,&move_count);
  assert(move_count>0);
  
  best_move_id = -1;
  
  if(move_count==1){
    *res = moves[0];
    return;
  } 
  
  if(depth_limit - look_ahead >= 2){
    std::cout << "Sorting... "; 
    std::cout.flush();
    sort_boards(moves,move_count,look_ahead);
    std::cout << "Done\n";
  }
  
  
  for(id=0;(int)id<move_count;id++){
    tmp_heur = alpha_beta(moves + id,best_heur,MAX_HEURISTIC,depth_limit);
    if(tmp_heur > best_heur){
      best_heur = tmp_heur;
      text = "move " + tostr<int>(id+1) + "/" + tostr<int>(move_count) +": heuristic == ";
      text += tostr<int>(best_heur) + "\n";
      std::cout << text;
      /* TODO update statusbar msg */
      
      best_move_id = id;
      *res = moves[id];
    }
    else{
      text = "move " + tostr<int>(id+1) + "/" + tostr<int>(move_count) +": heuristic <= ";
      text += tostr<int>(best_heur) + "\n";
      std::cout << text;
      /* TODO update statusbar msg */
    }
  }

  /* bot can not prevent losing all discs -> just pick a move */
  if(best_heur == MIN_HEURISTIC){ 
    *res = moves[0];
  }


  time_diff = (std::time(NULL)-prev_move_time);
  std::cout << nodes << " nodes in " << time_diff << " seconds: ";
  std::cout << nodes/(time_diff==0 ? 1 : time_diff) << " nodes / sec\n";
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
  CRASH;
  return 0;
}

void bot_base::sort_boards(board *moves,int move_count, int depth_limit)
{
  bool loop;
  int i,*heur,best_heur = MIN_HEURISTIC;
  
  heur = new int[move_count];
  
  for(i=0;i<move_count;i++){
    heur[i] = alpha_beta(moves + i,best_heur,MAX_HEURISTIC,depth_limit);
    if(heur[i] > best_heur){
      best_heur = heur[i];
    }
  }
  do{
    loop = false;
    for(i=0;i<move_count-1;i++){
      if(heur[i] < heur[i+1]){
        std::swap(heur[i],heur[i+1]);
        std::swap(moves[i],moves[i+1]);
        loop = true;
      }
      assert(heur[i] >= heur[i+1]);
    }
  }while(loop);
  
  delete[] heur;
}
  
  
