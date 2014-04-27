#include "bot_ali.hpp"
#include "book/book.hpp"

const int bot_ali::location_values[10] =
{347,-39,-23,-40,-119,-35,-33,-10,-7,-5};

bot_ali::bot_ali(int sd, int pd):
  bot_base(sd,pd),
  book(BOOK_PATH + "book.csv")
{
  name = "bot_ali";
  shell_output = use_book = true;
  
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
  
  book.add_from_moves_file(BOOK_PATH + "moves.csv");
  
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
  
  struct timeval start;
  eval_mode mode;
  int child_count;
  board children[32];
  
  
  int empty_fields = b->get_empty_fields().count();
  
  if(shell_output){
    nodes = 0;
    gettimeofday(&start,NULL);
  }
  
  child_count = b->get_children(children) - children;
  assert(child_count != 0);
  

  
  book_t::citer it = book.data.find(b->to_database_string());
  
 
  if(child_count == 1){
    mode = ONE_MOVE_MODE;
  }  
  else if(empty_fields > perfect_depth){
    if(use_book && (it != book.data.end()) && (it->second.depth >= search_depth)){
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


    // is used for small negamax search, to sort moves before big search
    negamax_max_non_empty_fields = 
        b->get_non_empty_fields().count() + search_depth - 4;


    
    if(search_depth > min_sort_depth){
      int heurs[32];
      for(int i=0;i<child_count;i++){
        inspected = children[i];
        heurs[i] = -pvs<false>(MIN_HEURISTIC,MAX_HEURISTIC);
      }
      sort_boards(children,heurs,child_count);
    }

    // is used for big negamax search
    negamax_max_non_empty_fields = b->get_non_empty_fields().count() + search_depth;

  }
  
  int best_heur,best_id=0;
  
  switch(mode){
    case NORMAL_MODE:
      best_heur = MIN_HEURISTIC;
      for(int id=0;id<child_count;++id){
        inspected = children[id];
        int cur_heur = -pvs<true>(MIN_HEURISTIC,-best_heur);
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
      /*if(use_book 
        && (b->get_non_empty_fields().size() < (unsigned)book_t::entry_max_discs)
        && (search_depth>=book_t::min_learn_depth)
      ){
        int move = book.get_move_index(b,res);
        book_t::citer it = book.data.find(b->to_database_string());
        if(it == book.data.end() || search_depth>it->second.depth){
          book_t::value bv;
          bv.best_move = move;
          bv.depth = search_depth;
          book.data[b->to_database_string()] = bv;      
        }
        book.add_to_book_file(b->to_database_string(),search_depth,move);
      }*/
      break;
    case PERFECT_MODE:
      best_heur = MIN_PERFECT_HEURISTIC;
      for(int id=0;id<child_count;++id){
        inspected = children[id];
        int cur_heur = -pvs_exact(MIN_PERFECT_HEURISTIC,-best_heur);
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
        *res = board(b->to_database_string());
        int rot = res->get_rotation(b);
        res->do_move(it->second.best_move);
        *res = res->rotate(rot);
        std::cout << "move found in book at depth " << it->second.depth << '\n';
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
    struct timeval end;
    gettimeofday(&end,NULL);
    double time_diff = (end.tv_sec + (end.tv_usec / 1000000.0)) -
    (start.tv_sec + (start.tv_usec / 1000000.0));

    std::cout << big_number(nodes) << " nodes in " << time_diff << " seconds: ";
    std::cout << big_number((long)(nodes/(time_diff<0.000001 ? 1 : time_diff))) << " nodes / sec\n";
  }
}

template<bool sorted>
int bot_ali::pvs(int alpha, int beta)
{
 
  nodes++;
  
  int depth_left = 
    (int)inspected.get_non_empty_fields().count() - negamax_max_non_empty_fields;
  
  if(sorted && (depth_left >= min_sort_depth)){
    return pvs<false>(alpha,beta);
  }
  
  if(depth_left == 0){
    return heuristic();
  }

  
  
  std::bitset<64> valid_moves = inspected.get_valid_moves();
  
  if(valid_moves.none()){
    if(inspected.passed){
      return EXACT_SCORE_FACTOR * inspected.get_disc_diff();    
    }
    else{
      inspected.passed = true;
      inspected.switch_turn();
      int heur = -pvs<sorted>(-beta,-alpha);
      inspected.switch_turn();
      inspected.passed = false;
      return heur;
    }
  }
  
   
    
  if(sorted){
    board children[32]; 
    int child_count = inspected.get_children(children) - children;
    int heur[32];
    
    for(int i=0;i<child_count;i++){
      heur[i] = -children[i].count_valid_moves();
    }
    sort_boards(children,heur,child_count);
    
    
    
    std::swap<board>(inspected,children[0]);
    int score = -pvs<sorted>(-beta,-alpha);
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
        score = -pvs<sorted>(-beta,-alpha);
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
  else{
    
    int move = find_first_set_64(valid_moves.to_ulong());
    board backup = inspected;  
    
    inspected.do_move(move);
    int score = -pvs<sorted>(-beta,-alpha);
    inspected = backup;

    if(score >= beta){
      return beta;
    }
    if(score >= alpha){
      alpha = score;
    }
    valid_moves.reset(move);
    
    while(valid_moves.any()){
      move = find_first_set_64(valid_moves.to_ulong());
      
      inspected.do_move(move);
      score = -pvs_null_window(-alpha-1);
      inspected = backup;
      
      if((alpha < score) && (score < beta)){
        inspected.do_move(move);
        score = -pvs<sorted>(-beta,-alpha);
        inspected = backup;
      }
      if(score >= beta){
        return beta;
      }
      if(score >= alpha){
        alpha = score;
      }
      valid_moves.reset(move);
    }
    return alpha;
  }
}

int bot_ali::pvs_null_window(int alpha)
{
  nodes++;
  
  int depth_left = 
  (int)inspected.get_non_empty_fields().count() - negamax_max_non_empty_fields;
  
  if(depth_left == 0){
    return heuristic();
  }

  
  std::bitset<64> valid_moves = inspected.get_valid_moves();
  
  if(valid_moves.none()){
    if(inspected.passed){
      return EXACT_SCORE_FACTOR * inspected.get_disc_diff();    
    }
    else{
      inspected.passed = true;
      inspected.switch_turn();
      int heur = -pvs_null_window(-(alpha+1));
      inspected.switch_turn();
      inspected.passed = false;
      return heur;
    }
  }
  
  
  
  while(valid_moves.any()){
    int move = find_first_set_64(valid_moves.to_ulong());
    std::bitset<64> undo_data;
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
    valid_moves.reset(move);
  }
  return alpha;
  
}



int bot_ali::pvs_exact(int alpha, int beta)
{
  
  nodes++;
  
  std::bitset<64> valid_moves = inspected.get_valid_moves();
  
  if(valid_moves.none()){
    if(inspected.passed){
      return inspected.get_disc_diff();    
    }
    else{
      inspected.passed = true;
      inspected.switch_turn();
      int heur = -pvs_exact(-beta,-alpha);
      inspected.switch_turn();
      inspected.passed = false;
      return heur;
    }
  }
  
  
  
  bool null_window = false;
  
  while(valid_moves.any()){
    
    int move = find_first_set_64(valid_moves.to_ulong());
    std::bitset<64> undo_data;
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
    valid_moves.reset(move);
  }
  return alpha;
}



void bot_ali::sort_boards(board *boards,int* heurs, int count)
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






int bot_ali::heuristic()
{

  /*int res = 0;
  
  for(int i=9;i>=0;--i){
    res += bot_ali::location_values[i] * (
       (inspected.me & board::location[i]).count()
       -(inspected.opp & board::location[i]).count()
    );
  }  
  
  return res;*/
  
  
#define LOCATION_HEUR(i) \
  bot_ali::location_values[i] * ( \
    (inspected.me & board::location[i]).count() \
    -(inspected.opp & board::location[i]).count() \
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
