#include "bot_ali.hpp"

bot_ali::bot_ali(color _c, int sd, int wdl, int pd):
  bot_base(_c,sd,wdl,pd)
{  
  int loc[TOTAL_FIELDS] =
  {
    0,1,2,3,3,2,1,0,
    1,4,5,6,6,5,4,1,
    2,5,7,8,8,7,6,5,
    3,6,8,9,9,8,6,3,
    3,6,8,9,9,8,6,3,
    2,5,7,8,8,7,6,5,
    1,4,5,6,6,5,4,1,
    0,1,2,3,3,2,1,0
  };
  
  for(int i=0;i<TOTAL_FIELDS;i++){
    location_bitsets[loc[i]].set(i);
  }
  
  /* this should be enough: 16 children avg for 
     whenever somebody is crazy enough to caculate 
     the perfect sore from the start (depth=60) */  
  
  name = "bot_ali";
}

void bot_ali::do_move(const board* b,board* res) 
{  
  
  nodes = 0;

  struct timeval start;
  gettimeofday(&start,NULL);
  
  int child_count;
  b->get_children(board_stack,&child_count);
  assert(child_count>0);
  
  /* assign first board to res so if we find that we cant prevent losing all
  discs we dont return uninitialized data and also if we have only one move
  we can just return */
  *res = board_stack[0];
  
  if(child_count==1){
    return;
  } 
  
  int empty_fields = TOTAL_FIELDS - (b->discs[BLACK] | b->discs[WHITE]).count();
  
  eval_mode mode;
  if(empty_fields > wdl_depth){
    mode = NORMAL_MODE;
  }
  else if(empty_fields > perfect_depth){
    mode = WDL_MODE;
  }
  else{
    mode = PERFECT_MODE;
  }
  
  int best_heur;
  
  std::cout << name << " searching at depth ";
  
  switch(mode){
    case NORMAL_MODE:   
      best_heur = MIN_HEURISTIC; 
      std::cout << search_depth;
      break;
    case WDL_MODE:      
      best_heur = -1;
      std::cout << empty_fields;
      break;
    case PERFECT_MODE:  
      best_heur = -TOTAL_FIELDS; 
      std::cout << empty_fields;
      break;
  }
  std::cout << std::endl;
  
  
  if(search_depth>6){
    int heurs[TOTAL_FIELDS/2];
    for(int id=child_count-1;id>=0;--id){
      heurs[id] = -negamax(board_stack+id,MIN_HEURISTIC,MAX_HEURISTIC,4);
    }
    b->get_children(board_stack,&child_count); //rebuild board stack
    sort_boards(board_stack,heurs,child_count);
  }
  
  for(int id=child_count-1;id>=0;--id){
    int cur_heur;
    switch(mode){
      case NORMAL_MODE:
        cur_heur = -negamax(board_stack+id,MIN_HEURISTIC,-best_heur,search_depth);
        break;
      case WDL_MODE: 
        cur_heur = -negamax_wdl(board_stack+id,-best_heur);
        break;
      case PERFECT_MODE:
        cur_heur = -negamax_exact(board_stack+id,-TOTAL_FIELDS,-best_heur);
        break;
    }
    if(cur_heur > best_heur){
      best_heur = cur_heur;
      *res = board_stack[id];
    }
    std::cout << "move " << (child_count-id) << "/" << (child_count);
    std::cout << ": ";
    switch(mode){
      case WDL_MODE:
        switch(best_heur){
          case -1: std::cout << "loss"; break;
          case  0: std::cout << "draw"; break;
          case  1: std::cout << "win"; break;
          default: CRASH; break;
        }
        break;
      default:
        std::cout << best_heur;
        break;
    }
    
    std::cout << std::endl;
  }
  
  struct timeval end;
  gettimeofday(&end,NULL);  
  double time_diff = (end.tv_sec + (end.tv_usec / 1000000.0)) - 
  (start.tv_sec + (start.tv_usec / 1000000.0));

  std::cout << nodes << " nodes in " << time_diff << " seconds: ";
  std::cout << (int)(nodes/(time_diff<0.000001 ? 1 : time_diff)) << " nodes / sec\n";
}

int bot_ali::negamax_wdl(board* b, int beta)
{
  return negamax_exact(b,-beta,1);
}




int bot_ali::negamax(board* b,int alpha, int beta,int depth_remaining)
{           
  nodes++; 
  if(depth_remaining==0){
    return b->turn==WHITE ? heuristic(b) : -heuristic(b);
  }  
  
  int move_count;
  b->get_children(b+1,&move_count);
  if(move_count==0){
    b->turn = opponent(b->turn);
    b->get_children(b+1,&move_count);
    if(move_count==0){
      return EXACT_SCORE_FACTOR * 
      (b->turn==WHITE ?  b->get_disc_diff() : -b->get_disc_diff());    
    }
    return -negamax(b+1,-beta,-alpha,depth_remaining-1);
  }
  
  if(depth_remaining > 5){
    int heur[32];
    for(int id=0;id<move_count;id++){
      heur[id] = heuristic(b+1+id);
    }
    sort_boards(b+1,heur,move_count);
  }
  
  for(int id=move_count-1;id>=0;id--){
    int value = -negamax(b+1+id,-beta,-alpha,depth_remaining-1);
    if(value>=beta){
      return beta;
    }
    if(value>=alpha){
      alpha = value;
    }
  }
  return alpha;
}

int bot_ali::negamax_exact(board* b,int alpha, int beta)
{           
  nodes++; 
  
  int move_count;
  b->get_children(b+1,&move_count);
  if(move_count==0){
    *(b+1) = *b;
    (b+1)->turn = opponent((b+1)->turn);
    if(!(b+1)->has_moves()){
      return (b->turn==WHITE ?  b->get_disc_diff() : -b->get_disc_diff());    
    }
    return -negamax_exact(b+1,-beta,-alpha);
  }
  
  for(int id=move_count-1;id>=0;id--){
    int value = -negamax_exact(b+1+id,-beta,-alpha);
    if(value>=beta){
      return beta;
    }
    if(value>=alpha){
      alpha = value;
    }
  }
  return alpha;
}


void bot_ali::sort_boards(board *boards,int* heurs, int count)
{
  /*bool loop;
  do{
    loop = false;
    for(int i=0;i<count-1;++i){
      if(heurs[i] > heurs[i+1]){
        std::swap(heurs[i],heurs[i+1]);
        std::swap(boards[i],boards[i+1]);
        loop = true;
      }
      assert(heurs[i] <= heurs[i+1]);
    }
  }while(loop);*/
  int best_id = 0;
  for(int i=1;i<count;++i){
    if(heurs[best_id] > heurs[i]){
      best_id = i;
    }
  }
  std::swap(boards[best_id],boards[0]);
}


int bot_ali::heuristic(const board* b)
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
     50, // 0
    - 4, // 1 
    - 2, // 2
    - 2, // 3
    -20, // 4
    - 2, // 5
    - 2, // 6
    - 1, // 7
    - 1, // 8
      0  // 9
  };

  int res = 0;
  
  for(int i=0;i<10;i++){
    res += open_loc_val[i] * (
       (b->discs[WHITE] & location_bitsets[i]).count()
      -(b->discs[BLACK] & location_bitsets[i]).count()
    );
  }
  
  return res;
}

bot_ali::~bot_ali(){}