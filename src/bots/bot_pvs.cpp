#include "bots/bot_pvs.hpp"


bot_pvs::bot_pvs()
{
}

bot_pvs::~bot_pvs()
{
}




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

template<bool sorted,bool exact>
int bot_pvs::pvs(int alpha, int beta)
{
  stats.inc_nodes();
  
  if((sorted) && (moves_left < search_max_sort_depth)){
    return pvs<false,exact>(alpha,beta);
  }
  
  if((!exact) && moves_left == 0){
    return heuristic();
  }
  
  if((!exact) && get_use_book()){
    book_t::value bv = book->lookup(&inspected,moves_left);
    if(bv.best_move != book_t::NOT_FOUND){
      return min(max(bv.heur,alpha),beta);
    }
  }
  
  bits64 valid_moves = inspected.get_valid_moves();
  
  if(valid_moves == 0ull){
    int heur;
    inspected.switch_turn();
    if(inspected.get_valid_moves() == 0ull){
      heur = -EXACT_SCORE_FACTOR * inspected.get_disc_diff();    
    }
    else{
      heur = -pvs<sorted,exact>(-beta,-alpha);
    }
    inspected.switch_turn();
    return heur;
  }
    
  board children[32]; 
  int child_count = inspected.get_children(children) - children;
    
  
  if(sorted){
    
    // this branch is almost never taken
    if(board::only_similar_siblings(children,child_count)){
      std::swap<board>(inspected,children[0]);
      moves_left--;
      int score = -pvs<sorted,exact>(-beta,-alpha);
      moves_left++;
      std::swap<board>(inspected,children[0]);
      return score;
    }
    
    do_sorting(children,child_count);
  }
  
  
  std::swap<board>(inspected,children[0]);
  moves_left--;
  int score = -pvs<sorted,exact>(-beta,-alpha);
  moves_left++;
  std::swap<board>(inspected,children[0]);
  
  if(score >= beta){
    return beta;
  }
  if(score >= alpha){
    alpha = score;
  }
      
    
    
  for(int i=1;i<child_count;i++){
    
    std::swap<board>(inspected,children[i]);
    
    moves_left--;
    score = -pvs_null_window(-alpha-1);
    if((alpha < score) && (score < beta)){
     score = -pvs<sorted,exact>(-beta,-score);
    } 
    moves_left++;
    
    std::swap<board>(inspected,children[i]);
      
    
    if(score >= beta){
      return beta;
    }
    if(score >= alpha){
      alpha = score;
    }
  }
  return alpha;
    
    
}

int bot_pvs::pvs_null_window(int alpha)
{
  stats.inc_nodes();
  
  if(moves_left == 0){
    return heuristic();
  }
  
  bits64 valid_moves = inspected.get_valid_moves();

  if(valid_moves == 0ull){
    int heur;
    inspected.switch_turn();
    if(inspected.get_valid_moves() == 0ull){
      heur = -EXACT_SCORE_FACTOR * inspected.get_disc_diff();    
    }
    else{
      heur = -pvs_null_window(-(alpha+1));
    }
    inspected.switch_turn();
    return heur;
  }
  
  while(valid_moves != 0ull){
    int move = bits64_find_first(valid_moves);
    bits64 undo_data = inspected.do_move(move);
    moves_left--;
    int score = -pvs_null_window(-(alpha+1));
    moves_left++;
    inspected.undo_move(move,undo_data);
    
    if(score > alpha){
      return alpha+1;
    }
    valid_moves &= bits64_reset[move];
  }
  return alpha;
  
}


int bot_pvs::pvs_exact_null_window(int alpha)
{
  stats.inc_nodes();
  
  bits64 valid_moves = inspected.get_valid_moves();

  if(valid_moves == 0ull){
    int heur;
    inspected.switch_turn();
    if(inspected.get_valid_moves() == 0ull){
      heur = -inspected.get_disc_diff();    
    }
    else{
      heur = -pvs_exact_null_window(-(alpha+1));
    }
    inspected.switch_turn();
    return heur;
  }

  int move = bits64_find_first(valid_moves);
  
  bits64 undo_data = inspected.do_move(move);
  int score = -pvs_exact_null_window(-(alpha+1));
  inspected.undo_move(move,undo_data);
  
  if(score > alpha){
    return alpha+1;
  }
  valid_moves &= bits64_reset[move];
  
  while(valid_moves != 0ull){
    
    move = bits64_find_first(valid_moves);
    
    undo_data = inspected.do_move(move);
    score = -pvs_exact_null_window(-alpha-1);
    inspected.undo_move(move,undo_data);
    
    if(score > alpha){
      return alpha+1;
    }
    valid_moves &= bits64_reset[move];
  }
  return alpha;
}

void bot_pvs::do_move_one_possibility(const board* b, board* res)
{
  board children[32];
  b->get_children(children);
  *res = children[0];
  set_last_move_heur(NO_HEUR_AVAILABLE);
  output() << "only one valid move found, evaluation skipped.\n";
}

bool bot_pvs::do_move_book(const board* b, board* res)
{
  if(get_use_book()){
    book_t::value lookup = book->lookup(b,get_search_depth());
    if(lookup.best_move != book_t::NOT_FOUND){
      *res = *b;
      res->do_move(lookup.best_move);
      output() << "bot_" << get_name() << " found best move (";
      output() << board::index_to_position(lookup.best_move);
      output() << ") in book at depth " << lookup.depth;
      output() << ", heuristic " << lookup.heur << '\n';
      set_last_move_heur(lookup.heur);
      return true;
    }
  }
  return false;
}

void bot_pvs::do_move_normally(const board* b, board* res)
{
  stats.start_timer();
  
  board children[32];
  int child_count = b->get_children(children) - children;
  
  if(board::only_similar_siblings(children,child_count)){
    output() << "bot_" << get_name() << " sees that all moves are similar.\n";
    set_last_move_heur(0);
    *res = children[0];
    return;
  }
  
  
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
  
  set_last_move_heur(best_heur);
  *res = children[best_id];
  
  if(get_use_book()){
    int move = b->get_move_index(res);
    book_t::value v(move,get_search_depth(),best_heur);
    
    if(book->add(b,&v)){
      output() << "board was added to book\n";
    }
  }
  
  
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
  set_last_move_heur(best_heur);
  
  stats.stop_timer();
  
  output() << big_number(stats.get_nodes()) << " nodes in ";
  output() << stats.get_seconds() << " seconds: ";
  output() << big_number(stats.get_nodes_per_second()) << " nodes / sec\n";
}




void bot_pvs::do_move(const board* b,board* res)
{
  if(b->count_valid_moves() == 1){
    do_move_one_possibility(b,res);
  }
  else if(do_move_book(b,res)){
    (void)0;
  }  
  else if(b->count_empty_fields() > get_perfect_depth()){
    do_move_normally(b,res);
  }
  else{
    do_move_perfectly(b,res);
  }
}


