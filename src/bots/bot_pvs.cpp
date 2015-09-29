#include "bots/bot_pvs.hpp"

  
template<bool exact>
void bot_pvs::search(const board* b, board* res)
{
  stats.start_timer();
  
  board children[32];
  board* child_end = b->get_children(children);
  
  
  output() << "bot_" << get_name() << " searching ";
  if(exact){
    output() << "perfectly at depth " << b->count_empty_fields() << '\n';
  }
  else{
    output() << "at depth " << get_search_depth() << '\n';
  }

  moves_left = get_search_depth();
  
  int best_heur = MIN_HEURISTIC;
  for(const board* child=children;child!=child_end;++child){
    moves_left--;
    int cur_heur = -pvs<exact>(MIN_HEURISTIC,-best_heur,child);        
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
int bot_pvs::pvs(int alpha, int beta,const board* b)
{
  if((!exact) && moves_left==0){
    return heuristic(b);
  }
 
  stats.inc_nodes();
  bits64 valid_moves = b->get_valid_moves();
  if(valid_moves.none()){
    board copy = *b;
    copy.switch_turn();
    int heur;
    if(copy.has_valid_moves()){
      heur = -pvs<exact>(-beta,-alpha,&copy);
    }
    else{
      return (exact ? -1 : -EXACT_SCORE_FACTOR) * copy.get_disc_diff();
    }
    return heur;
  }
  
  board children[32];
  board* child_end = b->get_children(children,valid_moves);
  for(const board* child=children;child!=child_end;++child){
    --moves_left;
    alpha = max(alpha,-pvs<exact>(-beta,-alpha,child));
    ++moves_left;
    if(alpha >= beta){
      alpha = beta;
      break;
    }
  }
  return alpha;
}


void bot_pvs::do_move(const board* b,board* res)
{
  if(b->count_valid_moves() == 1){
    board children[32];
    b->get_children(children);
    *res = children[0];
    std::cout << "Only one valid move, evaluation skipped.\n";
  }
  else if(b->count_empty_fields() > get_perfect_depth()){
    search<false>(b,res);
  }
  else{
    search<true>(b,res);
  }
}


