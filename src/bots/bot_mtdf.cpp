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
  return;
  int heur[32];
  
  int tmp = moves_left;
  moves_left = 2;
  
  for(int i=0;i<child_count;i++){
    std::swap(inspected,children[i]);
    moves_left--;
    // WARNING: do not use mtdf() here, it messes up the hashtable
    heur[i] = -heuristic(); 
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
  moves_left = get_search_depth();
  

  
  
  int best_heur = MIN_HEURISTIC;
  int first_guess = heuristic();
  
  for(int id=0;id<child_count;++id){
    inspected = children[id];
    moves_left--;
    int cur_heur = mtdf<true,exact>(id==0 ? first_guess  : best_heur,best_heur);
    moves_left++;
    if(cur_heur > best_heur){
      best_heur = cur_heur;
      *res = children[id];
    }
    output() << "move " << (id+1) << "/" << (child_count);
    output() << " (" << board::index_to_position(b->get_move_index(children+id)) << ')';
    
    output() << ": " << best_heur << '\n';    
  }
  
  set_last_move_heur(best_heur);
  
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
  discs_on_searched_board = b->count_discs();
  
  if(b->count_valid_moves() == 1){
    do_move_one_possibility(b,res);
    return;
  }
  else if(do_move_book(b,res)){
    (void)0;
  }  
  else if(b->count_empty_fields() > get_perfect_depth()){
    hash_table.clear();
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
  while(upper_bound > lower_bound){
    beta = g + ((g==lower_bound) ? 1 : 0);
    g = -null_window<sort,exact>(-beta);
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
  if(sort){
    return null_window<false,exact>(alpha);
  }
  
  
  
  stats.inc_nodes();
  
  if((!exact) && moves_left == 0){
    return heuristic();
  }

  if(sort && ((!exact && moves_left<NORMAL_MOVE_SORT_DEPTH) || (exact && moves_left<PERFECT_MOVE_SORT_DEPTH))){
    return null_window<false,exact>(alpha); 
  }
  
  int move = -1;
  
 
  
  
  if(suitable_hashtable_entry<exact>()){
    hash_table_t::iterator ht_iter = hash_table.find(inspected);
    if(ht_iter != hash_table.end()){
      if(ht_iter->second.lower_bound > alpha){
        return alpha+1;
      }
      if(ht_iter->second.upper_bound <= alpha){
        return alpha;
      }
      move = ht_iter->second.best_move;
      if(move != -1){
        bits64 undo_data = inspected.do_move(move);
        moves_left--;
        int score = -null_window<sort,exact>(-alpha-1);
        
        
        moves_left++;
        inspected.undo_move(bits64_set[move],undo_data);
        if(score > alpha){
          move = ht_iter->second.best_move;
          return update_hash_table<exact>(alpha,alpha+1,move);
        }
      }
    }
  }
  
  bits64 valid_moves = inspected.get_valid_moves();

  
  if(move != -1){
    valid_moves &= bits64_reset[move];
    if(valid_moves == 0ull){
      return update_hash_table<exact>(alpha,alpha,move);
    }
  }
  
  
  
  
  if(valid_moves == 0ull){
    int heur;
    inspected.switch_turn();
    if(inspected.get_valid_moves() == 0ull){
      inspected.switch_turn();
      heur = inspected.get_disc_diff();
      if(!exact){
        heur *= EXACT_SCORE_FACTOR; 
      }
    }
    else{
      heur = -null_window<sort,exact>(-alpha-1);
      assert(heur==alpha || heur==alpha+1);
      inspected.switch_turn();
    }
    return update_hash_table<exact>(alpha,alpha+(heur>alpha),move);
  }
  

  
  if(sort){
    board children[32]; 
    int child_count = inspected.get_children(children,valid_moves) - children;
    
    do_sorting(children,child_count);
    for(int i=0;i<child_count;++i){
      move = children[i].get_move_index(&inspected);
      std::swap(inspected,children[i]);
      moves_left--;
      int score = -null_window<sort,exact>(-alpha-1);
      moves_left++;
      std::swap(inspected,children[i]);
      if(score > alpha){
        return update_hash_table<exact>(alpha,alpha+1,move);
      }
    }
    
    
    
    
  }
  else{
    for(int i=0;i<9;i++){
      bits64 location_moves = valid_moves & board::ordered_locations[i];
      while(location_moves != 0ull){
        move = bits64_find_first(location_moves);
        bits64 undo_data = inspected.do_move(move);
        moves_left--;
        int score = -null_window<sort,exact>(-alpha-1);
        moves_left++;
        inspected.undo_move(bits64_set[move],undo_data);
        location_moves &= bits64_reset[move];
        if(score > alpha){
          return update_hash_table<exact>(alpha,alpha+1,move);
        }
      }
    }
  }

  return update_hash_table<exact>(alpha,alpha,move);
}


template<bool exact>
inline int bot_mtdf::update_hash_table(int alpha, int res, int move)
{
  if(suitable_hashtable_entry<exact>()){
    auto it = hash_table.find(inspected);
    if(it == hash_table.end()){
      it = hash_table.insert(std::pair<const board,ht_data>(inspected,ht_data())).first;
      it->second.lower_bound = exact ? MIN_PERFECT_HEURISTIC : MIN_HEURISTIC;
      it->second.upper_bound = exact ? MAX_PERFECT_HEURISTIC : MAX_HEURISTIC;
    }
    it->second.best_move = move;
    
    if(res > alpha){
      it->second.lower_bound = max(it->second.lower_bound,alpha+1);
    }
    else{
      it->second.upper_bound = min(it->second.upper_bound,alpha);
    }
  }
  
  return res;
}

template<bool exact>
inline bool bot_mtdf::suitable_hashtable_entry(){
  int moves_done = inspected.count_discs() - discs_on_searched_board;
  
  if(exact){
    return moves_done<=8;
  }
  else{
    return moves_done<=6;
  }
}

void bot_mtdf::on_new_game()
{
  bot_base::on_new_game();
  hash_table.clear();
}


