#include "bots/bot_pvs.hpp"


void bot_pvs::do_sorting(board* children, int child_count)
{
  int heur[32];
  
  int tmp = moves_left;
  moves_left = 2;
  
  for(int i=0;i<child_count;i++){
    std::swap(inspected,children[i]);
    moves_left--;
    heur[i] = -pvs<false,false>(MIN_HEURISTIC,MAX_HEURISTIC);
    moves_left++;
    std::swap(inspected,children[i]);
  }
  
  ugly_sort<board>(children,heur,child_count);
  moves_left = tmp;
}

template<bool sort,bool exact>
int bot_pvs::pvs(int alpha, int beta)
{
  
  if(sort){
    if(exact && (moves_left < NORMAL_MOVE_SORT_DEPTH)){
      return pvs<false,true>(alpha,beta);
    }
    if((!exact) && (moves_left < PERFECT_MOVE_SORT_DEPTH)){
      return pvs<false,false>(alpha,beta);
    }
  }

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
    
  
  if(sort){
    do_sorting(children,child_count);
  }
  
  int score;
  for(int i=0;i<child_count;i++){
    std::swap<board>(inspected,children[i]);
    moves_left--;
    if(i==0){
      score = -pvs<sort,exact>(-beta,-alpha);
    }
    else{
      //if(exact){
      //  score = -pvs_exact_null_window(-alpha-1);
      //}
      //else{
        score = -pvs_null_window<exact>(-alpha-1);
      //}
      if((alpha < score) && (score < beta)){
        score = -pvs<sort,exact>(-beta,-score);
      } 
    }
    moves_left++;
    
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

void bot_pvs::do_move_normally(const board* b, board* res)
{
  do_move_search<false>(b,res);
}


template<bool exact>
void bot_pvs::do_move_search(const board* b, board* res)
{
  stats.start_timer();
  
  board children[32];
  int child_count = b->get_children(children) - children;
  
  
  output() << "bot_" << get_name() << " searching at depth ";
  output() << get_search_depth() << '\n';

  if(get_search_depth() > NORMAL_MOVE_SORT_DEPTH){
    
    moves_left = NORMAL_MOVE_SORT_DEPTH;
    
    int heurs[32];
    for(int i=0;i<child_count;i++){
      inspected = children[i];
      moves_left--;
      heurs[i] = -pvs<false,false>(MIN_HEURISTIC,MAX_HEURISTIC);
      moves_left++;
    }
    ugly_sort<board>(children,heurs,child_count);
    
  }

  moves_left = get_search_depth();
  

  
  int best_heur,best_id=0;
  
  best_heur = MIN_HEURISTIC;
  for(int id=0;id<child_count;++id){
    inspected = children[id];
    moves_left--;
    int cur_heur = -pvs<true,false>(MIN_HEURISTIC,-best_heur);
    moves_left++;
    if(cur_heur > best_heur){
      best_heur = cur_heur;
      best_id = id;
    }
    output() << "move " << (id+1) << "/" << (child_count);
    output() << ": " << best_heur << std::endl;
    
  }
  
  *res = children[best_id];
  
  stats.stop_timer();
  
  output() << big_number(stats.get_nodes()) << " nodes in ";
  output() << stats.get_seconds() << " seconds: ";
  output() << big_number(stats.get_nodes_per_second()) << " nodes / sec\n";
  
}

void bot_pvs::do_move_perfectly(const board* b, board* res)
{
  stats.start_timer();
  
  board children[32];
  int child_count = b->get_children(children) - children;
  
  output() << "bot_" << get_name() << " searching perfectly at depth ";
  output() << b->count_empty_fields() << '\n';
  
  
  if(moves_left > PERFECT_MOVE_SORT_DEPTH){
    
    moves_left = PERFECT_MOVE_SORT_DEPTH;
    
    int heurs[32];
    for(int i=0;i<child_count;i++){
      inspected = children[i];
      heurs[i] = -pvs<false,true>(MIN_HEURISTIC,MAX_HEURISTIC);
    }
    ugly_sort<board>(children,heurs,child_count);
    
  }
  
  moves_left = 64 - inspected.count_discs();
  
  int best_id = 0;
  int best_heur = -64000;
  for(int id=0;id<child_count;++id){
    inspected = children[id];
    int cur_heur = -pvs<true,true>(-64000,-best_heur);
    if(cur_heur > best_heur){
      best_heur = cur_heur;
      best_id = id;
    }
    output() << "move " << (id+1) << "/" << (child_count);
    output() << ": " << best_heur/1000 << std::endl;
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
    do_move_normally(b,res);
  }
  else{
    do_move_perfectly(b,res);
  }
}


