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
  // TODO implement this properly
  (void)children;
  (void)child_count;
  return;
}

void bot_mtdf::do_move_one_possibility(const board* b, board* res)
{
  board children[32];
  b->get_children(children);
  *res = children[0];
  output() << "only one valid move found, evaluation skipped.\n";
}

template<bool exact>
void bot_mtdf::do_move_search(const board* b, board* res)
{
  stats.start_timer();
  last_search_exact = exact;
 
  board children[32];
  int child_count = b->get_children(children) - children;
  
  output() << "bot_" << get_name() << " searching ";
  if(exact){
    output() << "perfectly at depth " << b->count_empty_fields() << '\n';
  }
  else{ 
    output() << "at depth " << get_search_depth() << '\n';
  }
  
  moves_left = exact ? get_perfect_depth() : get_search_depth();
  
  do_sorting(children,child_count);
  
  
  int best_heur = exact ? MIN_PERFECT_HEURISTIC : MIN_HEURISTIC;
  int first_guess;
  {
    int tmp = moves_left - 6;
    if(tmp < 0){
      first_guess = heuristic();
    }
    else{
      std::swap(tmp,moves_left);
      first_guess = mtdf<false,false>(0,best_heur);
      std::swap(tmp,moves_left);
    }
  }
  
  
  
  
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
  if(b->count_empty_fields() > get_perfect_depth()){
    hash_table.clear();
    do_move_search<false>(b,res);
  }
  else{
    if(!last_search_exact){
      hash_table.clear();
    }
    do_move_search<true>(b,res);
  }

    
  std::cout << "ht size = " << hash_table.size() << '\n';
  std::map<int,int,std::greater<int>> histo;
  for(const auto& x:hash_table){
    ++histo[x.second.uses];
  }
  for(const auto& x: histo){
    std::cout << x.first << " -> " << x.second << '\n';
  }
  
  
  
  
}

template<bool sort,bool exact>
int bot_mtdf::mtdf(int f,int lower_bound)
{
  int g = f;
  int upper_bound = MAX_HEURISTIC;
  while(upper_bound > lower_bound){
    int beta = g + ((g==lower_bound) ? (exact ? 2 : 1) : 0);
    std::cout << "g = " << g << '\n';
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
      ++ht_iter->second.uses;
      if(ht_iter->second.lower_bound > alpha){
        return alpha+1;
      }
      if(ht_iter->second.upper_bound <= alpha){
        return alpha;
      }
      move = ht_iter->second.best_move;
      if(move != -1){
        bits64 undo_data = inspected.do_move(move);
        if(!exact){
          moves_left--;
        }
        int score = -null_window<sort,exact>(-alpha-1);
        if(!exact){
          moves_left++;
        }
        inspected.undo_move(bits64::mask_set[move],undo_data);
        if(score > alpha){
          move = ht_iter->second.best_move;
          return update_hash_table<exact>(alpha,alpha+1,move);
        }
      }
    }
  }
  
  bits64 valid_moves = inspected.get_valid_moves();

  
  if(move != -1){
    valid_moves &= bits64::mask_reset[move];
    if(valid_moves.none()){
      return update_hash_table<exact>(alpha,alpha,move);
    }
  }
  
  
  
  
  if(valid_moves.none()){
    int heur;
    inspected.switch_turn();
    if(inspected.get_valid_moves().none()){
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
      if(!exact){
        moves_left--;
      }
      int score = -null_window<sort,exact>(-alpha-1);
      if(!exact){
        moves_left++;
      }
      std::swap(inspected,children[i]);
      if(score > alpha){
        return update_hash_table<exact>(alpha,alpha+1,move);
      }
    }
    
    
    
    
  }
  else{
    for(int i=0;i<9;i++){
      bits64 location_moves = valid_moves & board::ordered_locations[i];
      while(location_moves.any()){
        bits64 bit = location_moves.first_bit();
        move = bit.only_bit_index();
        bits64 undo_data = inspected.do_move(move);
        if(!exact){
          moves_left--;
        }
        int score = -null_window<sort,exact>(-alpha-1);
        if(!exact){
          moves_left++;
        }
        inspected.undo_move(bit,undo_data);
        location_moves ^= bit;
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
      it->second.uses = 0;
    }
    else{
      ++it->second.uses;
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
  (void)exact;
  return moves_done<=8;
}

void bot_mtdf::on_new_game()
{
  bot_base::on_new_game();
}


