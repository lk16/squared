#include "bot_ali.hpp"

const int bot_ali::location_values[10] =
{347,-39,-23,-40,-119,-35,-33,-10,-7,-5};

bot_ali::bot_ali(int _c, int sd, int pd):
  bot_base(_c,sd,pd)
{
  name = "bot_ali";
  shell_output = true;
  
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


void bot_ali::do_move(const board* b,board* res)
{
  
  struct timeval start;
  
  
  if(shell_output){
    nodes = 0;
    gettimeofday(&start,NULL);
  }
  
  int child_count;
  board children[32];
  
  
  child_count = b->get_children(children) - children;
  
  assert(child_count != 0);
  

#if !BOT_ALI_ALWAYS_SHOW_HEUR
  if(child_count==1){
    *res = children[0];
    return;
  }
#endif
  
  int empty_fields = b->get_empty_fields().count();
  eval_mode mode = (empty_fields > perfect_depth) ? NORMAL_MODE : PERFECT_MODE;

  int best_heur = (mode==NORMAL_MODE) ? MIN_HEURISTIC : (-64);
  
  // if we cannot prevent losing all discs, we at least have valid data to return
  int best_id = 0;
  
  if(shell_output){
    std::cout << name << " searching at depth ";
    std::cout << ((mode==NORMAL_MODE) ? search_depth : empty_fields) << '\n';
  }


  // is used for small negamax search, to sort moves before big search
  negamax_max_non_empty_fields = b->get_non_empty_fields().count() + search_depth - 4;


  
  if(search_depth>BOT_ALI_MIN_SEARCH_DEPTH_TO_SORT){
    int heurs[32];
    for(int i=0;i<child_count;i++){
      inspected = children[i];
      heurs[i] = -pvs<false>(MIN_HEURISTIC,MAX_HEURISTIC);
    }
    sort_boards(children,heurs,child_count);
  }

  // is used for big negamax search
  negamax_max_non_empty_fields = b->get_non_empty_fields().count() + search_depth;

  for(int id=0;id<child_count;++id){
    inspected = children[id];
    int cur_heur;
    switch(mode){
      case NORMAL_MODE:
        cur_heur = -pvs<true>(MIN_HEURISTIC,-best_heur);
        break;
      case PERFECT_MODE:
        cur_heur = -pvs_exact(-64,-best_heur);
        break;
    }
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

  
  if(shell_output){
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
  
  if(sorted){
    if(depth_left < 7){
      return pvs<false>(alpha,beta);
    }
  }
  else{
    if(depth_left == 0){
      return heuristic();
    }
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
    int heur[32];
    int child_count = inspected.get_children(children) - children;
    for(int i=0;i<child_count;i++){
      heur[i] = -children[i].count_valid_moves();
    }
    sort_boards(children,heur,child_count);
    
    
    bool null_window = false;
    
    for(int i=0;i<child_count;i++){
      
      int score;
      
      
      if(null_window){
        std::swap<board>(inspected,children[i]);
        score = -pvs<sorted>(-alpha-1,-alpha);
        std::swap<board>(inspected,children[i]);
        
        if((alpha < score) && (score < beta)){
          std::swap<board>(inspected,children[i]);
          score = -pvs<sorted>(-beta,-alpha);
          std::swap<board>(inspected,children[i]);
        }
      }
      else{
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
      null_window = true;
    }
    return alpha;
    
    
  }
  else{
    bool null_window = false;
    
    while(valid_moves.any()){
      int move = find_first_set_64(valid_moves.to_ulong());
      std::bitset<64> undo_data;
      int score;
      
      
      if(null_window){
        undo_data = inspected.do_move(move);
        score = -pvs<sorted>(-alpha-1,-alpha);
        inspected.undo_move(move,undo_data);
        
        if((alpha < score) && (score < beta)){
          undo_data = inspected.do_move(move);
          score = -pvs<sorted>(-beta,-alpha);
          inspected.undo_move(move,undo_data);
        }
      }
      else{
        undo_data = inspected.do_move(move);
        score = -pvs<sorted>(-beta,-alpha);
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

  int res = 0;
  
  for(int i=9;i>=0;--i){
    res += bot_ali::location_values[i] * (
       (inspected.me & board::location[i]).count()
       -(inspected.opp & board::location[i]).count()
    );
  }  
  
  return res;
}

bot_ali::~bot_ali(){}
