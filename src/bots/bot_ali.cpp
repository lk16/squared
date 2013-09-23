#include "bot_ali.hpp"

bot_ali::bot_ali(color _c, int sd, int wdl, int pd):
  bot_base(_c,sd,wdl,pd)
{  
  int loc[64] =
  {
    0,1,2,3,3,2,1,0,
    1,4,5,6,6,5,4,1,
    2,5,7,8,8,7,5,2,
    3,6,8,9,9,8,6,3,
    3,6,8,9,9,8,6,3,
    2,5,7,8,8,7,5,2,
    1,4,5,6,6,5,4,1,
    0,1,2,3,3,2,1,0
  };
  
  for(int i=0;i<64;i++){
    location_bitsets[loc[i]].set(i);
  }
  
  name = "bot_ali";
  shell_output = true;
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
  b->get_children(children,&child_count);
  assert(child_count>0);
  
  

#if !BOT_ALI_ALWAYS_SHOW_HEUR
  if(child_count==1){
    *res = children[0];
    return;
  } 
#endif 
  
  int empty_fields = 64 - (b->discs[BLACK] | b->discs[WHITE]).count();
  eval_mode mode = (empty_fields > wdl_depth) ? NORMAL_MODE : PERFECT_MODE;

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
    int cur_heur;
    inspected = children[id];
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
    int heur = heuristic();
    if(inspected.turn==BLACK){
      heur = -heur;
    }
    return heur;
  }  
  
  int move_count=0;
  std::bitset<64> undo_data,possible_moves;
  
  inspected.get_possible_moves(&possible_moves);
  
  
  while(true){
    int move = find_first_set_64(possible_moves.to_ulong());
    if(move==0){
      break;
    }
    move--;
    if(inspected.try_move(move,&undo_data)){
      int value = -negamax(-beta,-alpha,depth_remaining-1);
      inspected.undo_move(move,&undo_data);
      if(value >= beta){
        return beta;
      }
      if(value >= alpha){
        alpha = value;
      }
      move_count++;
    }
    possible_moves.reset(move);
  }
  if(move_count!=0){
    return alpha;
  }
  else{
    inspected.switch_turn();
    inspected.get_children(NULL,&move_count);
    int heur;
    if(move_count!=0){
      heur = -negamax(-beta,-alpha,depth_remaining-1);
      inspected.switch_turn();
      return heur;
    }
    else{
      // at this point inspected.turn is switched
      inspected.switch_turn();
      heur = EXACT_SCORE_FACTOR * inspected.get_disc_diff();
      if(inspected.turn==BLACK){
        heur = -heur;
      }
      return heur;
    }
  }
  
}




int bot_ali::negamax_exact(int alpha, int beta)
{           
  
  nodes++; 
  
  int move_count=0;
  std::bitset<64> undo_data,possible_moves;
  
  inspected.get_possible_moves(&possible_moves);
  
  while(true){
    int move = find_first_set_64(possible_moves.to_ulong());
    if(move==0){
      break;
    }
    move--;
    if(inspected.try_move(move,&undo_data)){
      int value = -negamax_exact(-beta,-alpha);
      inspected.undo_move(move,&undo_data);
      if(value >= beta){
        return beta;
      }
      if(value >= alpha){
        alpha = value;
      }
      move_count++;
    }
    possible_moves.reset(move);
  }
  if(move_count!=0){
    return alpha;
  }
  else{
    if((inspected.discs[BLACK] | inspected.discs[WHITE]).count()!=64){
      inspected.switch_turn();
      inspected.get_children(NULL,&move_count);
      int heur;
      if(move_count!=0){
        heur = -negamax_exact(-beta,-alpha);
        inspected.switch_turn();
        return heur;
      }
      inspected.switch_turn();
    }
    return (inspected.turn==WHITE ?  inspected.get_disc_diff() : 
      -inspected.get_disc_diff());    
  }
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

  static int open_loc_val[10] = {
      400, // 0
     - 70, // 1 
     - 40, // 2
     - 40, // 3
     -150, // 4
     - 35, // 5
     - 30, // 6
     - 15, // 7
     - 10, // 8
     -  5  // 9
  };

  int res = 0;
  
  for(int i=0;i<10;i++){
    res += open_loc_val[i] * (
       (inspected.discs[WHITE] & location_bitsets[i]).count()
       -(inspected.discs[BLACK] & location_bitsets[i]).count()
    );
  }
  
  return res;
}

bot_ali::~bot_ali(){}