#include "bots/bot_pvs.hpp"

template<bool sort,bool exact>
int bot_pvs::pvs(int alpha, int beta)
{
  stats.inc_nodes();

  
  if((!exact) && moves_left == 0){
    return heuristic();
  }
  
  bits64 moves = inspected.get_valid_moves();
  if(moves.none()){
    inspected.switch_turn();
    if(!inspected.has_valid_moves()){
      int diff = inspected.get_disc_diff();
      inspected.switch_turn();
      return -EXACT_SCORE_FACTOR * diff;  
    }
    int heur = -pvs<sort,exact>(-beta,-alpha);
    inspected.switch_turn();
    return heur;
  }
    
    
  board children[32]; 
  int child_count = inspected.get_children(children) - children;
    
  int score;
  for(int i=0;i<child_count;i++){
    std::swap<board>(inspected,children[i]);
    if(!exact){
      moves_left--;
    }
    if(i==0){
      score = -pvs<sort,exact>(-beta,-alpha);
    }
    else{
      score = -pvs_null_window<exact>(-alpha-1);
      if((alpha < score) && (score < beta)){
        score = -pvs<sort,exact>(-beta,-score);
      } 
    }
    if(!exact){
      moves_left++;
    }
    std::swap<board>(inspected,children[i]);
      
    
    if(score >= beta){
      return beta;
    }
    alpha = max(alpha,score);
  }
  return alpha;
    
    
}

template<bool exact>
int bot_pvs::pvs_null_window(int alpha)
{
  stats.inc_nodes();
  
  if((!exact) && moves_left == 0){
    return heuristic();
  }
  
  bits64 valid_moves = inspected.get_valid_moves();

  if(valid_moves.none()){
    int heur;
    inspected.switch_turn();
    if(inspected.get_valid_moves().none()){
      heur = -EXACT_SCORE_FACTOR * inspected.get_disc_diff();    
    }
    else{
      heur = -pvs_null_window<exact>(-alpha-1);
    }
    inspected.switch_turn();
    return heur;
  }
  for(int i=0;i<9;i++){
    bits64 location_moves = valid_moves & board::ordered_locations[i];
    while(location_moves.any()){
      bits64 bit = location_moves.first_bit();
      int move = bit.only_bit_index();
      bits64 undo_data = inspected.do_move(move);
      moves_left--;
      int score = -pvs_null_window<exact>(-alpha-1);
      moves_left++;
      inspected.undo_move(bit,undo_data);
      if(score > alpha){
        return alpha+1;
      }
      location_moves ^= bit;
    }
  }
  return alpha;
  
}


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
    int cur_heur = -pvs<true,false>(MIN_HEURISTIC,-best_heur);
    if(!exact){ 
      moves_left++;
    }
    if(cur_heur > best_heur){
      best_heur = cur_heur;
      best_id = id;
    }
    output() << "move " << (id+1) << "/" << (child_count) << ": ";
    output() << (exact ? best_heur/EXACT_SCORE_FACTOR : best_heur) << '\n';
    
  }     
  
  *res = children[best_id];
  
  stats.stop_timer();
  
  output() << big_number(stats.get_nodes()) << " nodes in ";
  output() << stats.get_seconds() << " seconds: ";
  output() << big_number(stats.get_nodes_per_second()) << " nodes / sec\n";
  
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
    do_move_search<false>(b,res);
  }
  else{
    do_move_search<true>(b,res);
  }
}


