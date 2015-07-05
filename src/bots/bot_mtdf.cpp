#include "bots/bot_mtdf.hpp"


bot_mtdf::bot_mtdf()
{
  set_name("mtdf");  
}


bot_mtdf::~bot_mtdf()
{
}

void bot_mtdf::do_sorting(board* children, int child_count)
{
  int heur[32];
  
  int tmp = moves_left;
  moves_left = 2;
  
  for(int i=0;i<child_count;i++){
    std::swap(inspected,children[i]);
    moves_left--;
    heur[i] = -mtdf<false,false>(0,MIN_HEURISTIC);
    moves_left++;
    std::swap(inspected,children[i]);
  }
  
  ugly_sort<board>(children,heur,child_count);
  moves_left = tmp;
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

template<bool exact>
void bot_mtdf::do_move_search(const board* b, board* res)
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
  
  
  output() << "bot_" << get_name() << " searching ";
  if(exact){
    output() << "perfectly at depth " << b->count_empty_fields() << '\n';
  }
  else{ 
    output() << "at depth " << get_search_depth() << '\n';
  }
  
  if(exact && (b->count_empty_fields() > PERFECT_MOVE_SORT_DEPTH)){
    do_sorting(children,child_count);
  }
  
  if((!exact) && get_search_depth() > NORMAL_MOVE_SORT_DEPTH){
    do_sorting(children,child_count);
  }

  moves_left = get_search_depth();
  

  
  int best_heur,best_id=0;
  
  best_heur = MIN_HEURISTIC;
  for(int id=0;id<child_count;++id){
    inspected = children[id];
    moves_left--;
    int cur_heur = -mtdf<true,exact>(id==0 ? 0 : best_heur,best_heur);
    moves_left++;
    if(cur_heur > best_heur){
      best_heur = cur_heur;
      best_id = id;
    }
    output() << "move " << (id+1) << "/" << (child_count);
    output() << ": " << best_heur << '\n';    
  }
  
  set_last_move_heur(best_heur);
  *res = children[best_id];
  
  if((!exact) && get_use_book()){
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



void bot_mtdf::do_move(const board* b,board* res)
{
  if(b->count_valid_moves() == 1){
    do_move_one_possibility(b,res);
    return;
  }
  else if(do_move_book(b,res)){
    (void)0;
  }  
  else if(b->count_empty_fields() > get_perfect_depth()){
    do_move_search<false>(b,res);
  }
  else{
    do_move_search<true>(b,res);
  }
}

template<bool sort,bool exact>
int bot_mtdf::mtdf(int f,int lower_bound)
{
  int g = f;
  int upper_bound = MAX_HEURISTIC;
  int beta;
  while(lower_bound < upper_bound){
    if(g == lower_bound){
      beta = g+1;
    }
    else{
      beta = g;
    }
    g = null_window<sort,exact>(beta-1);
    if(g < beta){
      upper_bound = g;
    }
    else{
      lower_bound = g;
    }
  }
  return g;
}

template<bool sort,bool exact>
int bot_mtdf::null_window(int alpha)
{
  // TODO: sorting 
  
  
  stats.inc_nodes();
  
  if((!exact) && moves_left == 0){
    return heuristic();
  }
  
  bits64 valid_moves = inspected.get_valid_moves();

  if(valid_moves == 0ull){
    int heur;
    inspected.switch_turn();
    if(inspected.get_valid_moves() == 0ull){
      heur = -inspected.get_disc_diff();
      if(!exact){
        heur *= EXACT_SCORE_FACTOR; 
      }
      heur = alpha + ((heur > alpha) ? 1 : 0);
    }
    else{
      heur = -null_window<sort,exact>(-(alpha+1));
    }
    inspected.switch_turn();
    return heur;
  }
  
  int move;
  
  for(int i=0;i<9;i++){
    bits64 location_moves = valid_moves & board::ordered_locations[i];
    while(location_moves != 0ull){
      bits64 move_bit = bits64_first(location_moves);
      location_moves &= ~move_bit;
      move = bits64_find_first(move_bit);
      bits64 undo_data = inspected.do_move(move);
      moves_left--;
      int score = -null_window<sort,exact>(-(alpha+1));
      moves_left++;
      inspected.undo_move(move_bit,undo_data);
      
      if(score > alpha){
        ++alpha;
        goto add_ht_input;
      }
    }
  }
  
  
  add_ht_input:
#if 0
  if(moves_left>=HASH_TABLE_MIN_DEPTH && moves_left<=HASH_TABLE_MAX_DEPTH){
    ht_data ht_item;
    ht_item.best_move = move;
    ht_item.lower_bound // THIS NEEDS REDESIGNING
  }
#endif
  return alpha;
}

