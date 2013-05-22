#include "bot_base.hpp"

#ifndef SQUARED_BOT_ENABLE_OUTPUT
#define SQUARED_BOT_ENABLE_OUTPUT 1
#endif

bot_base::bot_base(color _c, int _max_depth, int _max_endgame_depth):
  max_depth(_max_depth),
  max_endgame_depth(_max_endgame_depth),
  look_ahead(3),
  c(_c),
  prev_move_time(std::time(NULL))
{
}

void bot_base::do_move(const board* b,board* res) 
{
  int depth_limit,alpha,move_count,best_move_id;
  board moves[TOTAL_FIELDS/2];  
  
  nodes = 0;
  alpha = MIN_HEURISTIC;
  best_move_id = -1;
  
#if SQUARED_BOT_ENABLE_OUTPUT
  struct timeval start;
  gettimeofday(&start,NULL);
#endif  
  
  depth_limit = (b->max_moves_left() <= max_endgame_depth) ? (TOTAL_FIELDS-4) : max_depth;
  
  b->get_children(moves,&move_count);
  assert(move_count>0);
  
  if(move_count==1){
    *res = moves[0];
    return;
  } 
  
  // TODO put look_ahead code in different method
  if(depth_limit - look_ahead >= 2){
#if SQUARED_BOT_ENABLE_OUTPUT
    std::cout << "Sorting... "; 
    std::cout.flush();
#endif
//    sort_boards(moves,move_count,look_ahead);
#if SQUARED_BOT_ENABLE_OUTPUT
    std::cout << "Done\n";
#endif    
  }

  for(int id=0;id<move_count;id++){
    int heur = -alpha_beta(moves + id,alpha,MAX_HEURISTIC,depth_limit);
    if(heur > alpha){
      alpha = heur;
      best_move_id = id;
    }
#if SQUARED_BOT_ENABLE_OUTPUT
    std::string text = "move " + tostr<int>(id+1) + "/" + tostr<int>(move_count) +": heuristic ";
    text += ((best_move_id==id) ? "== " : "<= ") + tostr<int>(alpha) + "\n";
    std::cout << text;
    /* TODO update statusbar msg */
#endif
  }

  /* if the bot cannot prevent losing all discs, just pick a move */
  *res = moves[(best_move_id == -1) ? 0 : best_move_id];
  
  
#if SQUARED_BOT_ENABLE_OUTPUT
  struct timeval end;
  gettimeofday(&end,NULL);  
  double time_diff = (end.tv_sec + (end.tv_usec / 1000000.0)) - 
  (start.tv_sec + (start.tv_usec / 1000000.0));

  std::cout << nodes << " nodes in " << time_diff << " seconds: ";
  std::cout << (int)(nodes/(time_diff<0.000001 ? 1 : time_diff)) << " nodes / sec\n";
#endif
}




int bot_base::alpha_beta(const board* b,int alpha, int beta,int depth_remaining)
{           
  nodes++; 
   
  if(b->test_game_ended()){
    return PERFECT_SCORE_FACTOR * (b->turn==WHITE ? 1 : -1) * b->get_disc_diff();    
  }
  if(depth_remaining==0){
    return (b->turn==WHITE ? 1 : -1) * heuristic(b);
  }  
  
  board children[32];
  int move_count;
  
  b->get_children(children,&move_count);
  if(move_count==0){
    board tmp(*b);
    tmp.turn = opponent(b->turn);
    return -alpha_beta(&tmp,-beta,-alpha,depth_remaining-1);
  }
  
  for(int id=0;id<move_count;id++){
    int value = -alpha_beta(children + id,-beta,-alpha,depth_remaining-1);
    if(value>=beta){
      return value;
    }
    if(value>=alpha){
      alpha = value;
    }
  }
  return alpha;
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

int bot_base::get_max_depth() const
{
  return max_depth;
}
