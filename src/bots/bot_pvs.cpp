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
  
  int tmp_search_max_discs = inspected.count_discs() + 2;
  std::swap(tmp_search_max_discs,search_max_discs);
  
  for(int i=0;i<child_count;i++){
    //heur[i] = -children[i].count_valid_moves();
    std::swap(inspected,children[i]);
    heur[i] = -pvs_unsorted(MIN_HEURISTIC,MAX_HEURISTIC);
    std::swap(inspected,children[i]);
  }
  
  std::swap(tmp_search_max_discs,search_max_discs);
  
  ugly_sort<board>(children,heur,child_count);
}


int bot_pvs::pvs_sorted(int alpha, int beta)
{
  stats.inc_nodes();
  
  int depth_left = inspected.count_discs() - search_max_discs;
  
  if(depth_left > search_max_sort_depth){
    return pvs_unsorted(alpha,beta);
  }
  
  if(depth_left == 0){
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
      heur = -pvs_unsorted(-beta,-alpha);
    }
    inspected.switch_turn();
    return heur;
  }
    
  board children[32]; 
  int child_count = inspected.get_children(children) - children;
  
  do_sorting(children,child_count);
  
  
  std::swap<board>(inspected,children[0]);
  int score = -pvs_sorted(-beta,-alpha);
  std::swap<board>(inspected,children[0]);
  
  if(score >= beta){
    return beta;
  }
  if(score >= alpha){
    alpha = score;
  }
      
    
    
  for(int i=1;i<child_count;i++){
    
    std::swap<board>(inspected,children[i]);
    
    score = -pvs_null_window(-alpha-1);
    if((alpha < score) && (score < beta)){
     score = -pvs_sorted(-beta,-score);
    } 
    
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


int bot_pvs::pvs_unsorted(int alpha, int beta)
{
  stats.inc_nodes();
    
  if(inspected.count_discs() >= search_max_discs){
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
      heur = -pvs_unsorted(-beta,-alpha);
    }
    inspected.switch_turn();
    return heur;
  }

  int move = bits64_find_first(valid_moves);
  bits64 undo_data = inspected.do_move(move);
  int score = -pvs_unsorted(-beta,-alpha);
  inspected.undo_move(move,undo_data);

  if(score >= beta){
    return beta;
  }
  if(score >= alpha){
    alpha = score;
  }
  valid_moves &= bits64_reset[move];
  
  while(valid_moves != 0ull){
    move = bits64_find_first(valid_moves);
    undo_data = inspected.do_move(move);
    
    score = -pvs_null_window(-alpha-1);
    if((alpha < score) && (score < beta)){
      score = -pvs_unsorted(-beta,-score);
    }
    
    inspected.undo_move(move,undo_data);
    
    if(score >= beta){
      return beta;
    }
    if(score >= alpha){
      alpha = score;
    }
    valid_moves &= bits64_reset[move];
  }
  return alpha;
}

int bot_pvs::pvs_null_window(int alpha)
{
  stats.inc_nodes();
  
  if(inspected.count_discs() >= search_max_discs){
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
    int score = -pvs_null_window(-(alpha+1));
    inspected.undo_move(move,undo_data);
    
    if(score > alpha){
      return alpha+1;
    }
    valid_moves &= bits64_reset[move];
  }
  return alpha;
  
}



int bot_pvs::pvs_exact(int alpha, int beta)
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
      heur = -pvs_exact(-beta,-alpha);
    }
    inspected.switch_turn();
    return heur;
  }

  int move = bits64_find_first(valid_moves);
  
  bits64 undo_data = inspected.do_move(move);
  int score = -pvs_exact(-beta,-alpha);
  inspected.undo_move(move,undo_data);
  
  if(score >= beta){
    return beta;
  }
  if(score >= alpha){
    alpha = score;
  }
  valid_moves &= bits64_reset[move];
  
  while(valid_moves != 0ull){
    
    move = bits64_find_first(valid_moves);
    
    undo_data = inspected.do_move(move);
    
    score = -pvs_exact_null_window(-alpha-1);
    if((alpha < score) && (score < beta)){
      score = -pvs_exact(-beta,-score);
    }
    
    inspected.undo_move(move,undo_data);
    
    if(score >= beta){
      return beta;
    }
    if(score >= alpha){
      alpha = score;
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
  output() << "only one valid move found, evaluation skipped.\n";
}

bool bot_pvs::do_move_book(const board* b, board* res)
{
  if(get_use_book()){
    book_t::value lookup = book->lookup(b,get_search_depth());
    if(lookup.best_move != book_t::NOT_FOUND){
      *res = *b;
      res->do_move(lookup.best_move);
      output() << "bot_" << get_name() << " found best move (" << lookup.best_move;
      output() << ") in book at depth " << lookup.depth << '\n';
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
  
  output() << "bot_" << get_name() << " searching at depth ";
  output() << get_search_depth() << '\n';

  search_max_discs = b->count_discs() + get_search_depth();

  
  if(get_search_depth() > search_max_sort_depth){
    
    search_max_discs -= 4;
    
    int heurs[32];
    for(int i=0;i<child_count;i++){
      inspected = children[i];
      heurs[i] = -pvs_sorted(MIN_HEURISTIC,MAX_HEURISTIC);
    }
    ugly_sort<board>(children,heurs,child_count);
    
    search_max_discs += 4;
  }

  int best_heur,best_id=0;
  
  best_heur = MIN_HEURISTIC;
  for(int id=0;id<child_count;++id){
    inspected = children[id];
    int cur_heur = -pvs_sorted(MIN_HEURISTIC,-best_heur);
    if(cur_heur > best_heur){
      best_heur = cur_heur;
      best_id = id;
    }
    output() << "move " << (id+1) << "/" << (child_count);
    output() << ": " << best_heur << std::endl;
    
  }
  
  *res = children[best_id];
  
  if(get_use_book()){
    if(book->add(b,res,get_search_depth())){
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
  
  search_max_discs = b->count_discs() + get_search_depth();

  
  if(get_search_depth() > search_max_sort_depth){
    
    search_max_discs -= 4;
    
    int heurs[32];
    for(int i=0;i<child_count;i++){
      inspected = children[i];
      heurs[i] = -pvs_sorted(MIN_HEURISTIC,MAX_HEURISTIC);
    }
    ugly_sort<board>(children,heurs,child_count);
    
    search_max_discs += 4;
  }
  
  int best_id = 0;
  int best_heur = MIN_PERFECT_HEURISTIC;
  for(int id=0;id<child_count;++id){
    inspected = children[id];
    int cur_heur = -pvs_exact(MIN_PERFECT_HEURISTIC,-best_heur);
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




void bot_pvs::do_move(const board* b,board* res)
{
  if(b->count_valid_moves() == 1){
    do_move_one_possibility(b,res);
    return;
  }
  
  if(do_move_book(b,res)){
    return;
  }
  
  if(b->count_empty_fields() > get_perfect_depth()){
    do_move_normally(b,res);
    return;
  }
  
  do_move_perfectly(b,res);

}

