#include "bots/bot_pvs.hpp"

int bot_pvs::look_ahead(const board* b)
{
  return -pvs<false>(MIN_HEURISTIC,MAX_HEURISTIC,b);
}

  
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

  if((!exact) && (moves_left >= LOOK_AHEAD_DEPTH + 3)){
    int swap_var;
    std::swap<int>(swap_var,moves_left);
    int best_estimation = look_ahead(&children[0]);
    for(board* child=children+1;child!=child_end;++child){
      int estimation = look_ahead(child);
      if(estimation > best_estimation){
        std::swap<board>(children[0],*child);
        best_estimation = estimation;
      }
    }
    std::swap<int>(swap_var,moves_left);
  }
  
  
  for(const board* child=children;child!=child_end;++child){
    --moves_left;
    int heur; 
    if(child == children){
      heur = -pvs<exact>(-beta,-alpha,child);
    }
    else{
      heur = -pvs<exact>(-alpha-1,-alpha,child);
      if(heur > alpha && heur < beta){
        heur = -pvs<exact>(-beta,-alpha-1,child);
      }
    }
    ++moves_left;
    if(heur > alpha){
      alpha = heur;
    }
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


