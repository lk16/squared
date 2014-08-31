#include "bots/bot_mtdf.hpp"


bot_mtdf::bot_mtdf()
{
  set_name("mtdf");  
}


bot_mtdf::~bot_mtdf()
{
}


void bot_mtdf::do_move_one_possibility(const board* b, board* res)
{
  board children[32];
  b->get_children(children);
  *res = children[0];
  output() << "only one valid move found, evaluation skipped.\n";
}

bool bot_mtdf::do_move_book(const board* b, board* res)
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

void bot_mtdf::do_move_normally(const board* b, board* res)
{
  stats.start_timer();
  
  board children[32];
  int child_count = b->get_children(children) - children;
  
  output() << "bot_" << get_name() << " searching at depth ";
  output() << get_search_depth() << '\n';

  /*if(get_search_depth() > NORMAL_MOVE_SORT_DEPTH){
    
    moves_left = NORMAL_MOVE_SORT_DEPTH;
    
    int heurs[32];
    for(int i=0;i<child_count;i++){
      inspected = children[i];
      moves_left--;
      heurs[i] = -pvs_unsorted(MIN_HEURISTIC,MAX_HEURISTIC);
      moves_left++;
    }
    ugly_sort<board>(children,heurs,child_count);
    
  }*/

  moves_left = get_search_depth();
  

  
  int best_heur,best_id=0;
  
  best_heur = MIN_HEURISTIC;
  for(int id=0;id<child_count;++id){
    inspected = children[id];
    moves_left--;
    int cur_heur = mtdf(0);
    moves_left++;
    if(cur_heur > best_heur){
      best_heur = cur_heur;
      best_id = id;
    }
    output() << "move " << (id+1) << "/" << (child_count);
    output() << ": " << best_heur << std::endl;
    
  }
  
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

void bot_mtdf::do_move_perfectly(const board* b, board* res)
{
  stats.start_timer();
  
  board children[32];
  int child_count = b->get_children(children) - children;
  
  output() << "bot_" << get_name() << " searching perfectly at depth ";
  output() << b->count_empty_fields() << '\n';
  
  
  /*if(moves_left > PERFECT_MOVE_SORT_DEPTH){
    
    moves_left = PERFECT_MOVE_SORT_DEPTH;
    
    int heurs[32];
    for(int i=0;i<child_count;i++){
      inspected = children[i];
      heurs[i] = -pvs_sorted(MIN_HEURISTIC,MAX_HEURISTIC);
    }
    ugly_sort<board>(children,heurs,child_count);
    
  }*/
  
  moves_left = 64 - inspected.count_discs();
  
  int best_id = 0;
  int best_heur = MIN_PERFECT_HEURISTIC;
  for(int id=0;id<child_count;++id){
    inspected = children[id];
    int cur_heur = mtdf(0);
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




void bot_mtdf::do_move(const board* b,board* res)
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

int bot_mtdf::mtdf(int f)
{
  int g = f;
  int upper_bound = MAX_HEURISTIC;
  int lower_bound = MIN_HEURISTIC;
  int beta;
  while(lower_bound < upper_bound){
    if(g == lower_bound){
      beta = g+1;
    }
    else{
      beta = g;
    }
    g = -null_window(beta-1);
    //std::cout << "g = " << g << "\tupperbound = " << upper_bound;
    //std::cout << "\tlowerbound = " << lower_bound << '\n';
    if(g < beta){
      upper_bound = g;
    }
    else{
      lower_bound = g;
    }
  }
  return g;
}

int bot_mtdf::null_window(int alpha)
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
      heur = -null_window(-(alpha+1));
    }
    inspected.switch_turn();
    return heur;
  }
  
  while(valid_moves != 0ull){
    int move = bits64_find_first(valid_moves);
    bits64 undo_data = inspected.do_move(move);
    moves_left--;
    int score = -null_window(-(alpha+1));
    moves_left++;
    inspected.undo_move(move,undo_data);
    
    if(score > alpha){
      return alpha+1;
    }
    valid_moves &= bits64_reset[move];
  }
  return alpha;
}

int bot_mtdf::mtdf_exact(int f)
{
  int g = f;
  int upper_bound = MAX_HEURISTIC;
  int lower_bound = MIN_HEURISTIC;
  int beta;
  while(lower_bound < upper_bound){
    if(g == upper_bound){
      beta = g+1;
    }
    else{
      beta = g;
    }
    g = null_window_exact(beta-1);
    if(g < beta){
      upper_bound = g;
    }
    else{
      lower_bound = g;
    }
  }
  return g;
}

int bot_mtdf::null_window_exact(int alpha)
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
      heur = -null_window_exact(-(alpha+1));
    }
    inspected.switch_turn();
    return heur;
  }

  int move = bits64_find_first(valid_moves);
  
  bits64 undo_data = inspected.do_move(move);
  int score = -null_window_exact(-(alpha+1));
  inspected.undo_move(move,undo_data);
  
  if(score > alpha){
    return alpha+1;
  }
  valid_moves &= bits64_reset[move];
  
  while(valid_moves != 0ull){
    
    move = bits64_find_first(valid_moves);
    
    undo_data = inspected.do_move(move);
    score = -null_window_exact(-alpha-1);
    inspected.undo_move(move,undo_data);
    
    if(score > alpha){
      return alpha+1;
    }
    valid_moves &= bits64_reset[move];
  }
  return alpha;
}


