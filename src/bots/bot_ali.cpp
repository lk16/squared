#include "bot_ali.hpp"
#include "book/book.hpp"

#include <fstream>
#include <algorithm>

bot_ali::bot_ali(int sd, int pd):
  bot_base(sd,pd),
  book(BOOK_PATH + "book.csv"),
  tpt(&board_hasher)
  
{
  name = "bot_ali";
  shell_output = use_book = true;
  int tmp[] = {347,-39,-23,-40,-119,-35,-33,-10,-7,-5};
  std::copy(tmp,tmp+10,location_values);
  
  
  /*
  0,1,2,3,3,2,1,0,
  1,4,5,6,6,5,4,1,
  2,5,7,8,8,7,5,2
  3,6,8,9,9,8,6,3,
  3,6,8,9,9,8,6,3,
  2,5,7,8,8,7,5,2
  1,4,5,6,6,5,4,1,
  0,1,2,3,3,2,1,0
  */
  
}

void bot_ali::disable_shell_output()
{
  shell_output = false;
}

void bot_ali::disable_book()
{
  use_book = false;
}


void bot_ali::do_move(const board* b,board* res)
{
  
  eval_mode mode;
  int child_count;
  board children[32];
  
  
  int empty_fields = b->count_empty_fields();
  
  stats.start_timer();
  
  child_count = b->get_children(children) - children;
  assert(child_count != 0);
  

  
  book_t::value lookup = book.lookup(b,search_depth);
  
 
  if(child_count == 1){
    mode = ONE_MOVE_MODE;
  }  
  else if(empty_fields > perfect_depth){
    if(use_book && (lookup.best_move != book_t::NOT_FOUND)){
      mode = BOOK_MODE;
    }
    else{
      mode = NORMAL_MODE;
    }
  }
  else{
    mode = PERFECT_MODE;
  }
  

  if(mode==NORMAL_MODE || mode==PERFECT_MODE){
  
    if(shell_output){
      
      std::cout << name << " searching at depth ";
      std::cout << ((mode==NORMAL_MODE) ? search_depth : empty_fields) << '\n';
    }


    // is used for small search, to sort moves before big search
    search_max_discs = b->count_discs() + search_depth - 4;


    
    if(search_depth > search_max_sort_depth){
      int heurs[32];
      for(int i=0;i<child_count;i++){
        inspected = children[i];
        heurs[i] = -pvs_sorted(MIN_HEURISTIC,MAX_HEURISTIC);
      }
      sort_children(children,heurs,child_count);
    }

    // is used for big search
    search_max_discs += 4;

  }
  
  int best_heur,best_id=0;
  
  switch(mode){
    case NORMAL_MODE:
      best_heur = MIN_HEURISTIC;
      for(int id=0;id<child_count;++id){
        inspected = children[id];
#if !BOT_ALI_USE_MTDF
        int cur_heur = -pvs_sorted(MIN_HEURISTIC,-best_heur);
#else
        int cur_heur = -id_mtdf(search_depth);
#endif
        if(cur_heur > best_heur){
          best_heur = cur_heur;
          best_id = id;
        }
        if(shell_output){
          std::cout << "move " << (id+1) << "/" << (child_count);
          std::cout << ": " << best_heur << std::endl;
        }
      }
      *res = children[best_id];
      if(use_book){        
        book.add(b,res,search_depth);     
      }
      break;
    case PERFECT_MODE:
      best_heur = MIN_PERFECT_HEURISTIC;
      for(int id=0;id<child_count;++id){
        inspected = children[id];
#if !BOT_ALI_USE_MTDF
        int cur_heur = -pvs_exact(MIN_PERFECT_HEURISTIC,-best_heur);
#else
        int cur_heur = -id_mtdf_exact();
#endif
        if(cur_heur > best_heur){
          best_heur = cur_heur;
          best_id = id;
        }
        if(shell_output){
          std::cout << "move " << (id+1) << "/" << (child_count);
          std::cout << ": " << best_heur << std::endl;
        }
      }
      *res = children[best_id];
      break;
    case BOOK_MODE:
      {
        *res = *b;
        res->do_move(lookup.best_move);
        if(shell_output){
          std::cout << "best move (" << lookup.best_move;
          std::cout << ") found in book at depth " << lookup.depth << '\n';
        }
      }
      break;
    case ONE_MOVE_MODE:
      *res = children[0];
      if(shell_output){
        std::cout << "only one valid move found, evaluation skipped.\n";
      }
      break;
  }
  
  if(shell_output && (mode==NORMAL_MODE || mode==PERFECT_MODE)){
    stats.stop_timer();
    
    std::cout << big_number(stats.get_nodes()) << " nodes in ";
    std::cout << stats.get_seconds() << " seconds: ";
    std::cout << big_number(stats.get_nodes_per_second()) << " nodes / sec\n";
  }
}

int bot_ali::pvs_sorted(int alpha, int beta)
{
  stats.inc_nodes();
  
  int depth_left = 
    (int)inspected.count_discs() - search_max_discs;
  
  if(depth_left > search_max_sort_depth){
    return pvs_unsorted(alpha,beta);
  }
  
  if(depth_left == 0){
    return heuristic();
  }
  
  std::bitset<64> valid_moves = inspected.get_valid_moves();
  
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
  
  sort_children(children,heur,child_count);
  
  
  
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
    std::swap<board>(inspected,children[i]);
    
    if((alpha < score) && (score < beta)){
      std::swap<board>(inspected,children[i]);
      score = -pvs_sorted(-beta,-alpha);
      std::swap<board>(inspected,children[i]);
    }
    
    if(score >= beta){
      return beta;
    }
    if(score >= alpha){
      alpha = score;
    }
  }
  return alpha;
    
    
}


int bot_ali::pvs_unsorted(int alpha, int beta)
{
  stats.inc_nodes();
    
  if(inspected.count_discs() == search_max_discs){
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
  board backup = inspected;  
  
  inspected.do_move(move);
  int score = -pvs_unsorted(-beta,-alpha);
  inspected = backup;

  if(score >= beta){
    return beta;
  }
  if(score >= alpha){
    alpha = score;
  }
  valid_moves &= bits64_reset[move];
  
  while(valid_moves != 0ull){
    move = bits64_find_first(valid_moves);
    
    inspected.do_move(move);
    score = -pvs_null_window(-alpha-1);
    inspected = backup;
    
    if((alpha < score) && (score < beta)){
      inspected.do_move(move);
      score = -pvs_unsorted(-beta,-alpha);
      inspected = backup;
    }
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

int bot_ali::pvs_null_window(int alpha)
{
  stats.inc_nodes();
  
  if(inspected.count_discs() == search_max_discs){
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
    bits64 undo_data;
    int score;
    
    undo_data = inspected.do_move(move);
    score = -pvs_null_window(-(alpha+1));
    inspected.undo_move(move,undo_data);
    
    if(score >= (alpha+1)){
      return (alpha+1);
    }
    if(score >= alpha){
      alpha = score;
    }
    valid_moves &= bits64_reset[move];
  }
  return alpha;
  
}



int bot_ali::pvs_exact(int alpha, int beta)
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

  bool null_window = false;
  
  while(valid_moves != 0ull){
    
    int move = bits64_find_first(valid_moves);
    bits64 undo_data;
    int score;
    
    
    if(null_window){
      undo_data = inspected.do_move(move);
      score = -pvs_exact(-alpha-1,-alpha);
      inspected.undo_move(move,undo_data);
      
      if((alpha < score) && (score < beta)){
        undo_data = inspected.do_move(move);
        score = -pvs_exact(-beta,-alpha);
        inspected.undo_move(move,undo_data);
      }
    }
    else{
      undo_data = inspected.do_move(move);
      score = -pvs_exact(-beta,-alpha);
      inspected.undo_move(move,undo_data);
    }
    
    if(score >= beta){
      return beta;
    }
    if(score >= alpha){
      alpha = score;
    }
    null_window = true;
    valid_moves &= bits64_reset[move];
  }
  return alpha;
}



void bot_ali::sort_children(board *boards,int* heurs, int count)
{
  bool loop;
  do{
    loop = false;
    for(int i=1;i<count;++i){
      if(heurs[i-1] < heurs[i]){
        std::swap(heurs[i-1],heurs[i]);
        std::swap(boards[i-1],boards[i]);
        loop = true;
      }
    }
  }while(loop);
}

int bot_ali::id_mtdf(int max_depth)
{
  int heur = 0;
  for(int d=1;d<max_depth;d++){
    heur = mtdf(heur,d);
  }
  return heur;
}

int bot_ali::id_mtdf_exact()
{
  int max_depth = inspected.get_empty_fields();
  int heur = 0;
  for(int d=1;d<max_depth;d++){
    heur = mtdf_exact(heur,d);
  }
  return heur;
}

int bot_ali::mtdf(int f,int depth){

  search_max_discs = inspected.count_discs() + depth;
  
  int g = f;
  int upper_bound = MAX_HEURISTIC;
  int lower_bound = MIN_HEURISTIC;
  while(lower_bound < upper_bound){
    
    int beta = ((g == lower_bound) ? g+1 : g);
    
    g = pvs_null_window_with_memory(beta-1);
    
    if(g < beta){
      upper_bound = g;
    }
    else{
      lower_bound = g;
    }
  }
  return g;
}

int bot_ali::mtdf_exact(int f,int depth){

  search_max_discs = inspected.count_discs() + depth;
  
  int g = f;
  int upper_bound = MAX_HEURISTIC;
  int lower_bound = MIN_HEURISTIC;
  while(lower_bound < upper_bound){
    
    int beta = ((g == lower_bound) ? g+1 : g);
    
    g = pvs_null_window_with_memory_exact(beta-1);
    
    if(g < beta){
      upper_bound = g;
    }
    else{
      lower_bound = g;
    }
  }
  return g;
}

bool bot_ali::set_location_values_from_file(const std::string& fname){
  std::fstream f(fname);
  int location_values_copy[10];
  
  if(!f.is_open()){
    return false;
  }
  
  for(int i=0;i<10;i++){
    if(!(f >> location_values_copy[i])){
      return false;
    }
  }

  set_location_values(location_values_copy);

  return true;
}

void bot_ali::set_location_values(const int* v)
{
  std::copy(v, v+10, location_values);
}




int bot_ali::heuristic()
{
  
#define LOCATION_HEUR(i) \
  bot_ali::location_values[i] * ( \
    count_64(inspected.me & board::location[i]) \
    - count_64(inspected.opp & board::location[i]) \
  )
  
  return
  LOCATION_HEUR(0) +
  LOCATION_HEUR(1) +
  LOCATION_HEUR(2) +
  LOCATION_HEUR(3) +
  LOCATION_HEUR(4) +
  LOCATION_HEUR(5) +
  LOCATION_HEUR(6) +
  LOCATION_HEUR(7) +
  LOCATION_HEUR(8) +
  LOCATION_HEUR(9);
  
#undef LOCATION_HEUR
}

bot_ali::~bot_ali(){}

void bot_ali::on_new_game()
{
  if(use_book){
    book.reload();
  }
}

int bot_ali::pvs_null_window_with_memory(int alpha)
{
  int beta = alpha+1;
  
  if(tpt_value* n = tpt.lookup(inspected)){
    
    // TODO use n->best_move
    
    if(n->lower_bound >= beta){
      return n->lower_bound;
    }
    if(n->upper_bound <= alpha){
      return n->upper_bound;
    }
    //alpha = max<int>(alpha,n->lower_bound); // this has no effect EVER
  }
  
  stats.inc_nodes();
  
  if(inspected.count_discs() == search_max_discs){
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
      heur = -pvs_null_window_with_memory(1-alpha);
    }
    inspected.switch_turn();
    return heur;
  }
  
  int g = MIN_HEURISTIC;
  int a = alpha;
  
  while((g < beta) && (valid_moves != 0ull)){
    int move = bits64_find_first(valid_moves);
    
    bits64 undo_data = inspected.do_move(move);
    g = max<int>(g,-pvs_null_window_with_memory(1-alpha));
    a = max<int>(a,g);
    inspected.undo_move(move,undo_data);
    
    valid_moves &= bits64_reset[move];
  }
  
  if(g <= alpha){
    tpt.add(inspected,tpt_value(MIN_HEURISTIC,g,-1));
  }
  if(g >= beta){
    tpt.add(inspected,tpt_value(g,MAX_HEURISTIC,-1));
  }
  return g;
}

int bot_ali::pvs_null_window_with_memory_exact(int alpha)
{
  int beta = alpha+1;
  
  if(tpt_value* n = tpt.lookup(inspected)){
    
    // TODO use n->best_move
    
    if(n->lower_bound >= beta){
      return n->lower_bound;
    }
    if(n->upper_bound <= alpha){
      return n->upper_bound;
    }
    //alpha = max<int>(alpha,n->lower_bound); // this has no effect EVER
  }
  
  stats.inc_nodes();
  
  bits64 valid_moves = inspected.get_valid_moves();

  if(valid_moves == 0ull){
    int heur;
    inspected.switch_turn();
    if(inspected.get_valid_moves() == 0ull){
      heur = -EXACT_SCORE_FACTOR * inspected.get_disc_diff();    
    }
    else{
      heur = -pvs_null_window_with_memory(1-alpha);
    }
    inspected.switch_turn();
    return heur;
  }
  
  int g = MIN_HEURISTIC;
  int a = alpha;
  
  while((g < beta) && (valid_moves != 0ull)){
    int move = bits64_find_first(valid_moves);
    
    bits64 undo_data = inspected.do_move(move);
    g = max<int>(g,-pvs_null_window_with_memory(1-alpha));
    a = max<int>(a,g);
    inspected.undo_move(move,undo_data);
    
    valid_moves &= bits64_reset[move];
  }
  
  if(g <= alpha){
    tpt.add(inspected,tpt_value(MIN_HEURISTIC,g,-1));
  }
  if(g >= beta){
    tpt.add(inspected,tpt_value(g,MAX_HEURISTIC,-1));
  }
  return g;
}

const int* bot_ali::get_location_values() const
{
  return location_values;
}
