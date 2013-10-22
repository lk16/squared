#include "bot_ali.hpp"

bot_ali::bot_ali(int _c, int sd, int pd):
  bot_base(_c,sd,pd)
{
  name = "bot_ali";
  shell_output = true;
}

void bot_ali::disable_shell_output()
{
  shell_output = false;
}

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

const int bot_ali::location_value[10] = {
   347, // 0
  - 39, // 1
  - 23, // 2
  - 40, // 3
  -119, // 4
  - 35, // 5
  - 33, // 6
  - 10, // 7
  - 7, // 8
  - 5 // 9
};



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



  
  if(search_depth>BOT_ALI_MIN_SEARCH_DEPTH_TO_SORT){
    int heurs[32];
    for(int i=0;i<child_count;i++){
      inspected = children[i];
      heurs[i] = -negamax(MIN_HEURISTIC,MAX_HEURISTIC,search_depth-4);
    }
    sort_boards(children,heurs,child_count);
  }


  for(int id=0;id<child_count;++id){
    inspected = children[id];
    int cur_heur;
    switch(mode){
      case NORMAL_MODE:
        cur_heur = -negamax(MIN_HEURISTIC,-best_heur,search_depth);
        break;
      case PERFECT_MODE:
        cur_heur = -negamax_exact(-64,-best_heur);
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

int bot_ali::negamax(int alpha, int beta, int depth_remaining)
{
 
  nodes++;
  
  if(depth_remaining==0){
    return heuristic();
  }
  
  std::bitset<64> possible_moves;
  
  inspected.get_valid_moves(&possible_moves);
  if(possible_moves.none()){
    if(inspected.passed){
      return EXACT_SCORE_FACTOR * inspected.get_disc_diff();    
    }
    else{
      inspected.passed = true;
      inspected.switch_turn();
      int heur = -negamax(-beta,-alpha,depth_remaining);
      inspected.switch_turn();
      inspected.passed = false;
      return heur;
    }
  }
  
  std::bitset<64> undo_data;
  
  while(true){
  
    int move = find_first_set_64(possible_moves.to_ulong());
    if(move == -1){
      break;
    }
    
    inspected.do_move(move,&undo_data);
    int value = -negamax(-beta,-alpha,depth_remaining-1);
    inspected.undo_move(move,&undo_data);
    
    if(value >= beta){
      return beta;
    }
    if(value >= alpha){
      alpha = value;
    }
    
    possible_moves.reset(move);
  }
  return alpha;
}




int bot_ali::negamax_exact(int alpha, int beta)
{
  
  nodes++;
  
  std::bitset<64> undo_data,possible_moves;
  
  inspected.get_valid_moves(&possible_moves);
  if(possible_moves.none()){
    if(inspected.passed){
      return inspected.get_disc_diff();    
    }
    else{
      inspected.passed = true;
      inspected.switch_turn();
      int heur = -negamax_exact(-beta,-alpha);
      inspected.switch_turn();
      inspected.passed = false;
      return heur;
    }
  }
  
  
  
  while(true){
    int move = find_first_set_64(possible_moves.to_ulong());
    if(move == -1){
      break;
    }
    inspected.do_move(move,&undo_data);
    int value = -negamax_exact(-beta,-alpha);
    inspected.undo_move(move,&undo_data);
    if(value >= beta){
      return beta;
    }
    if(value >= alpha){
      alpha = value;
    }
    possible_moves.reset(move);
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
  
  for(int i=0;i<10;i++){
    res += bot_ali::location_value[i] * (
       (inspected.me & board::location[i]).count()
       -(inspected.opp & board::location[i]).count()
    );
  }  
  return res;
}

bot_ali::~bot_ali(){}