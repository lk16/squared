#include "bots/bot_pvs.hpp"

  
template<bool exact>
void bot_pvs::do_move_search(const board* b, board* res)
{
  stats.start_timer();
  
  board children[32];
  board* child_end = b->get_children(children);
  
  
  output() << "bot_" << get_name() << " searching ";
  if(exact){
    output() << "perfectly at depth " << inspected.count_empty_fields() << '\n';
  }
  else{
    output() << "at depth " << get_search_depth() << '\n';
  }

  moves_left = get_search_depth();
  
  int best_heur = MIN_HEURISTIC;
  for(const board* child=children;child!=child_end;++child){
    inspected = *child;
    moves_left--;
    int cur_heur = -alpha_beta<exact>(MIN_HEURISTIC,-best_heur);        
    moves_left++;
    if(cur_heur > best_heur){
      best_heur = cur_heur;
      *res = *child;
    }
    output() << "move " << (child-children+1) << "/" << (child_end-children) << ": ";
    output() << best_heur << '\n';
    
  }     

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
 
  stats.inc_nodes();
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
  for(int i=0;i<(child_end-children);++i){
    std::swap(inspected,children[i]);
    --moves_left;
    alpha = max(alpha,-alpha_beta<exact>(-beta,-alpha));
    ++moves_left;
    std::swap(inspected,children[i]);
    if(alpha >= beta){
      alpha = beta;
      break;
    }
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


