#include "bots/bot_pvs.hpp"

int bot_pvs::look_ahead(const board* b)
{
  return -pvs<false,false>(MIN_HEURISTIC,MAX_HEURISTIC,b);
}

  
template<bool exact>
void bot_pvs::search(const board* b, board* res)
{
  moves_left = exact ? b->count_empty_fields() : get_search_depth();
  
  {
    int book_heur,pv;
    if(lookup_book(b,moves_left,&book_heur,&pv)){
      output() << "Found board in book, heuristic = " << book_heur;
      *res = *b;
      res->do_move(pv);
      return;
    }
  }
  
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

  
  const int worst_heur = exact ? MIN_PERFECT_HEURISTIC : MIN_HEURISTIC;
  int best_heur = worst_heur;

  for(const board* child=children;child!=child_end;++child){
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

  add_to_book(b,moves_left,best_heur,b->get_move_index(res));
  
  stats.stop_timer();
  
  output() << big_number(stats.get_nodes()) << " nodes in ";
  output() << stats.get_seconds() << " seconds: ";
  output() << big_number(stats.get_nodes_per_second()) << " nodes / sec\n";
  
}

template<bool exact,bool sorted>
int bot_pvs::pvs(int alpha, int beta,const board* b)
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
  
  board children[32];
  board* child_end = b->get_children(children,valid_moves);

  if((!exact) && sorted && (moves_left >= MIN_SORT_DEPTH)){
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
  
  if(sorted){
    for(const board* child=children;child!=child_end;++child){
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
      if(heur > alpha){
        alpha = heur;
      }
      if(alpha >= beta){
        alpha = beta;
        break;
      }
    }
  }
  else{
    board child;
    int heur;
    while(valid_moves.any()){
      bits64 move_bit = valid_moves.first_bit();
      int move = move_bit.only_bit_index();
      --moves_left;
      child = *b;
      child.do_move(move);
      heur = -pvs<exact,sorted>(-beta,-alpha,&child);
      ++moves_left;
      if(heur > alpha){
        alpha = heur;
      }
      if(alpha >= beta){
        return beta;
      }
      valid_moves ^= move_bit;
    }
  }
  return alpha;
}

template<bool exact>
int bot_pvs::pvs_null_window(int alpha,const board* b)
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
  }
  else if(b->count_empty_fields() > get_perfect_depth()){
    search<false>(b,res);
  }
  else{
    search<true>(b,res);
  }
}


