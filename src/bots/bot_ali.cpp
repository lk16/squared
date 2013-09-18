#include "bot_ali.hpp"

bot_ali::bot_ali(color _c, int sd, int wdl, int pd):
  bot_base(_c,sd,wdl,pd)
{  
  int loc[TOTAL_FIELDS] =
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
  
  for(int i=0;i<TOTAL_FIELDS;i++){
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
  b->get_children(board_stack,&child_count);
  assert(child_count>0);
  
  /* assign first board to res so if we find that we cant prevent losing all
  discs we dont return uninitialized data and also if we have only one move
  we can just return */
  *res = board_stack[0];

#if !BOT_ALI_ALWAYS_SHOW_HEUR
  if(child_count==1){
    return;
  } 
#endif 
  
  int empty_fields = TOTAL_FIELDS - (b->discs[BLACK] | b->discs[WHITE]).count();
  
  eval_mode mode;
  if(empty_fields > wdl_depth){
    mode = NORMAL_MODE;
  }
  else{
    mode = PERFECT_MODE;
  }
  
  int best_heur;
  
  if(shell_output){
    std::cout << name << " searching at depth ";
    switch(mode){
      case NORMAL_MODE:   
        std::cout << search_depth;
        break;   
      case PERFECT_MODE:  
        std::cout << empty_fields;
        break;
    }
    std::cout << std::endl;
  
#if 0
    std::cout << "Heuristic of this board: " << heuristic(b) << std::endl;
    std::cout << "Black value of this board: " << b->discs[BLACK].to_ulong() << std::endl; 
    std::cout << "White value of this board: " << b->discs[WHITE].to_ulong() << std::endl;
    std::cout << "Turn of this board: " << b->turn << std::endl;
#endif

  }
  switch(mode){
    case NORMAL_MODE:   
      best_heur = MIN_HEURISTIC; 
      break;
    case PERFECT_MODE:  
      best_heur = -TOTAL_FIELDS; 
      break;
  }
  
  
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
      case PERFECT_MODE:
        cur_heur = -negamax_exact(board_stack+id,-TOTAL_FIELDS,-best_heur);
        break;
    }
    if(cur_heur > best_heur){
      best_heur = cur_heur;
      *res = board_stack[id];
    }
    
    if(shell_output){
      std::cout << "move " << (child_count-id) << "/" << (child_count);
      std::cout << ": ";
      switch(mode){
        default:
          std::cout << best_heur;
          break;
      }
      
      std::cout << std::endl;
    }
  }
#if BOT_ALI_USE_HASHTABLE 
  table.clear();
#endif  
  if(shell_output){
    struct timeval end;
    gettimeofday(&end,NULL);  
    double time_diff = (end.tv_sec + (end.tv_usec / 1000000.0)) - 
    (start.tv_sec + (start.tv_usec / 1000000.0));

    std::cout << big_number(nodes) << " nodes in " << time_diff << " seconds: ";
    std::cout << big_number((long)(nodes/(time_diff<0.000001 ? 1 : time_diff))) << " nodes / sec\n";
  }
}

int bot_ali::negamax(board* b,int alpha, int beta,int depth_remaining)
{           
  nodes++; 
#if BOT_ALI_USE_HASHTABLE 
  if(search_depth-depth_remaining>=4 && search_depth-depth_remaining<=7){
    std::unordered_map<board,int>::iterator it = table.find(*b);
    if(it!=table.end()){
      return it->second;
    }
  }
#endif
  
  
  
  if(depth_remaining==0){
    int heur = (b->turn==WHITE) ? heuristic(b) : -heuristic(b);
#if BOT_ALI_USE_HASHTABLE 
    if(search_depth-depth_remaining>=4 && search_depth-depth_remaining<=7){
      table.insert(std::pair<board,int>(*b,heur));
    }
#endif
    return heur;
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
  
  for(int id=move_count-1;id>=0;id--){
    int value = -negamax(b+1+id,-beta,-alpha,depth_remaining-1);
    if(value>=beta){
      return beta;
    }
    if(value>=alpha){
      alpha = value;
    }
  }
#if BOT_ALI_USE_HASHTABLE 
  if(search_depth-depth_remaining>=4 && search_depth-depth_remaining<=7){
    table.insert(std::pair<board,int>(*b,alpha));
  }
#endif
  return alpha;
}


int bot_ali::negamax_stack(board* stack,int alpha, int beta,int max_depth)
{
#if 1
  depth_data_t depth_data[max_depth];
  int move_count;
  
  stack[0].get_children(stack+1,&move_count);
  if(move_count==0){
    stack[0].turn = opponent(stack[0].turn);
    stack[0].get_children(stack+1,&move_count);
    if(move_count==0){
      return EXACT_SCORE_FACTOR * 
      (stack[0].turn==WHITE ?  stack[0].get_disc_diff() : -stack[0].get_disc_diff());    
    }
    return -negamax_stack(stack,-beta,-alpha,max_depth);
  }
  
  
  depth_data[0].alpha = alpha;
  depth_data[0].beta = beta;
  depth_data[0].child_left = move_count;
  depth_data[0].child_start = 1;
  
  int next_it_depth = 0;
  int cur_depth = 0;
  int inspected = move_count;
  
  
  while(next_it_depth==-1){
    cur_depth = next_it_depth;
    depth_data_t* cdd = depth_data+cur_depth;
    if(cur_depth==max_depth){
      // TODO set right var to (b->turn==WHITE ?  b->get_disc_diff() : -b->get_disc_diff());    
      next_it_depth = cur_depth-1;
    }
    else{
      stack[inspected].get_children(stack + (cdd+1)->child_start,&move_count);
      if(move_count!=0){
        next_it_depth = cur_depth+1;
        depth_data[next_it_depth].alpha = -cdd->beta;
        depth_data[next_it_depth].beta  = -cdd->alpha;
        depth_data[next_it_depth].child_start = cdd->child_start+cdd->child_left;
        depth_data[next_it_depth].child_left = move_count;
      }
      else{
        stack[inspected+1] = stack[inspected];
        stack[inspected+1].turn = opponent(stack[inspected+1].turn);
        stack[inspected+1].get_children(NULL,&move_count);
        if(move_count!=0){
          depth_data[next_it_depth].alpha = cdd->alpha;
          depth_data[next_it_depth].beta  = cdd->beta;
          depth_data[next_it_depth].child_start = cdd->child_start+cdd->child_left;
          depth_data[next_it_depth].child_left = 1;
          next_it_depth = cur_depth+1;
        }
        else{
          // TODO set right var to (b->turn==WHITE ?  b->get_disc_diff() : -b->get_disc_diff());    
          next_it_depth = cur_depth-1;
        }
      }
    }
      
  }
  
  
  if(depth_remaining==0){
    int heur = (b->turn==WHITE) ? heuristic(b) : -heuristic(b);
    return heur;
  }  
  
  int move_count;
  
  
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
#else
  return 0;
#endif
}


int bot_ali::negamax_exact(board* b,int alpha, int beta)
{           
  nodes++; 
  
  int move_count;
  b->get_children(b+1,&move_count);
  if(move_count==0){
    *(b+1) = *b;
    (b+1)->turn = opponent((b+1)->turn);
    (b+1)->get_children(b+2,&move_count);
    if(move_count==0){
      return (b->turn==WHITE ?  b->get_disc_diff() : -b->get_disc_diff());    
    }
    return -negamax_exact(b+1,-beta,-alpha);
  }
  
  if( (~b->discs[BLACK] & ~b->discs[WHITE]).count() > 15){ 
    int heur[32];
    for(int id=0;id<move_count;id++){
      int tmp;
      b->get_children(NULL,&tmp);
      heur[id] = -tmp;
    }
    sort_boards(b+1,heur,move_count);
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
  bool loop;
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
  }while(loop);
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
      400, // 0
     - 70, // 1 
     - 40, // 2
     - 40, // 3
     -150, // 4
     - 35, // 5
     - 30, // 6
     - 20, // 7
     - 10, // 8
     -  5  // 9
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