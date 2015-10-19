#include "bots/bot_pvs.hpp"

int bot_pvs::look_ahead(board* b)
{
  return -pvs<false,false>(MIN_HEURISTIC,MAX_HEURISTIC,b);
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
  
  const int worst_heur = exact ? MIN_PERFECT_HEURISTIC : MIN_HEURISTIC;
  int best_heur = worst_heur;

  for(board* child=children;child!=child_end;++child){
    moves_left--;
    int cur_heur = -pvs<exact,true>(worst_heur,-best_heur,child);        
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

void bot_pvs::sort_children(board* children, board* child_end)
{
  int swap_var = ESTIMATE_DEPTH;
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


template<bool exact,bool sorted>
int bot_pvs::pvs(int alpha, int beta,board* b)
{
  if((!exact) && moves_left==0){
    return heuristic(b);
  }
  
  if(sorted && moves_left < MIN_SORT_DEPTH){
    return pvs<exact,false>(alpha,beta,b);
  }
  
  
  stats.inc_nodes();
  bits64 valid_moves = b->get_valid_moves();
  if(valid_moves.none()){
    board copy = *b;
    copy.switch_turn();
    if(copy.has_valid_moves()){
      return -pvs<exact,sorted>(-beta,-alpha,&copy);
    }
    else{
      return (exact ? -1 : -EXACT_SCORE_FACTOR) * copy.get_disc_diff();
    }
  }
  

  
  
  if(sorted){
    board children[32];
    board* child_end = b->get_children(children,valid_moves);

    if(!exact){
      sort_children(children,child_end);
    }
    
    for(board* child=children;child!=child_end;++child){
      --moves_left;
      int heur; 
      if(child == children){
        heur = -pvs<exact,sorted>(-beta,-alpha,child);
      }
      else{
        heur = -pvs_null_window<exact>(-alpha-1,child);
        if(heur > alpha && heur < beta){
          heur = -pvs<exact,sorted>(-beta,-heur,child);
        }
      }
      ++moves_left;
      if(alpha >= beta){
        alpha = beta;
        break;
      }
      if(heur > alpha){
        alpha = heur;
      }
    }
  }
  else{
    board child = *b;
    bits64 undo_data;
    int heur;
    bool first = true;
    while(valid_moves.any()){
      bits64 move_bit = valid_moves.first_bit();
      int move = move_bit.only_bit_index();
      --moves_left;
      undo_data = child.do_move(move);
      if(first){
        heur = -pvs<exact,sorted>(-beta,-alpha,&child);
        first = false;
      }
      else{
        heur = -pvs_null_window<exact>(-alpha-1,&child);
        if(heur > alpha && heur < beta){
          heur = -pvs<exact,sorted>(-beta,-heur,&child);
        }
      }
      child.undo_move(move_bit,undo_data);
      ++moves_left;
      if(alpha >= beta){
        alpha = beta;
        break;
      }
      if(heur > alpha){
        alpha = heur;
      }
      valid_moves ^= move_bit;
    }
  }
  return alpha;
}

template<bool exact>
int bot_pvs::pvs_null_window(int alpha,board* b)
{
  
  if((!exact) && moves_left==0){
    return heuristic(b) > alpha ? (alpha+1) : alpha;
  }
 
  stats.inc_nodes();
  bits64 valid_moves = b->get_valid_moves();
  if(valid_moves.none()){
    board copy = *b;
    copy.switch_turn();
    if(copy.has_valid_moves()){
      return -pvs_null_window<exact>(-alpha-1,&copy);
    }
    else{
      int heur = (exact ? -1 : -EXACT_SCORE_FACTOR) * copy.get_disc_diff();
      return (heur > alpha) ? (alpha+1) : alpha;
    }
  }
  board child;
  while(valid_moves.any()){
    bits64 move_bit = valid_moves.first_bit();
    int move = move_bit.only_bit_index();
    --moves_left;
    child = *b;
    child.do_move(move);
    int heur = -pvs_null_window<exact>(-alpha-1,&child); 
    ++moves_left;
    if(heur > alpha){
      return alpha+1;
    }
    valid_moves ^= move_bit;
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
    return;
  }
  board copy(*b);
  if(b->count_empty_fields() > get_perfect_depth()){
    search<false>(&copy,res);
  }
  else{
    search<true>(&copy,res);
  }
}


