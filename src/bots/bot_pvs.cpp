#include "bots/bot_pvs.hpp"

  
template<bool exact>
void bot_pvs::do_move_search(const board* b, board* res)
{
  stats.start_timer();
  
  board children[32];
  int child_count = b->get_children(children) - children;
  
  
  output() << "bot_" << get_name() << " searching ";
  if(exact){
    output() << "perfectly at depth " << get_perfect_depth() << '\n';
  }
  else{
    output() << "at depth " << get_search_depth() << '\n';
  }

  moves_left = get_search_depth();
  
  int best_heur,best_id=0;
  
  best_heur = MIN_HEURISTIC;
  for(int id=0;id<child_count;++id){
    inspected = children[id];
    if(!exact){
      moves_left--;
    }
    //int cur_heur = -pvs<true,exact>(MIN_HEURISTIC,-best_heur);
    int cur_heur = -alpha_beta<exact>(MIN_HEURISTIC,-best_heur);        
    if(!exact){ 
      moves_left++;
    }
    if(cur_heur > best_heur){
      best_heur = cur_heur;
      best_id = id;
    }
    output() << "move " << (id+1) << "/" << (child_count) << ": ";
    output() << best_heur << '\n';
    
  }     
  
  *res = children[best_id];
  
  stats.stop_timer();
  
  output() << big_number(stats.get_nodes()) << " nodes in ";
  output() << stats.get_seconds() << " seconds: ";
  output() << big_number(stats.get_nodes_per_second()) << " nodes / sec\n";
  
}

template<bool exact>
int bot_pvs::alpha_beta(int alpha, int beta)
{
  if((!exact) && moves_left==0){
    return heuristic();
  }
  
  
  bits64 valid_moves = inspected.get_valid_moves();
  if(valid_moves.none()){
    inspected.switch_turn();
    int heur;
    if(inspected.has_valid_moves()){
      heur = -alpha_beta<exact>(-beta,-alpha);
    }
    else{
      return (exact ? -1 : -EXACT_SCORE_FACTOR) * inspected.get_disc_diff();
    }
    inspected.switch_turn();
    return heur;
  }
  
  board children[32];
  board* child_end = inspected.get_children(children);
  int best = alpha;
  for(int i=0;i<(child_end-children);++i){
    std::swap(inspected,children[i]);
    --moves_left;
    alpha = max(alpha,-alpha_beta<exact>(-beta,-alpha));
    if(alpha >= beta){
      alpha = beta;
      break;
    }
    ++moves_left;
    std::swap(inspected,children[i]);
  }
  return alpha;
}


void bot_pvs::do_move(const board* b,board* res)
{
  /*if(b->count_valid_moves() == 1){
    board children[32];
    b->get_children(children);
    *res = children[0];
    std::cout << "Only one valid move, evaluation skipped.\n";
  }
  else */if(b->count_empty_fields() > get_perfect_depth()){
    do_move_search<false>(b,res);
  }
  else{
    do_move_search<true>(b,res);
  }
}


