#include "bot_ali.hpp"
#include <string.h>

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
  board_stack = new board[1000];
  
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
  
  std::cout << name << " searching at depth ";
  std::cout << ((empty_fields <= perfect_depth) ? empty_fields : search_depth) << '\n';
  
  
  if(search_depth>6 && (empty_fields>perfect_depth)){
    int heurs[TOTAL_FIELDS/2];
    for(int id=child_count-1;id>=0;--id){
      heurs[id] = -negamax(board_stack+id,MIN_HEURISTIC,MAX_HEURISTIC,4);
    }
    b->get_children(board_stack,&child_count); //rebuild board stack
    sort_boards(board_stack,heurs,child_count);
  }
  
    
  int cur_heur;
  int best_heur = (empty_fields <= perfect_depth) ? (-TOTAL_FIELDS) : MIN_HEURISTIC;
  for(int id=child_count-1;id>=0;--id){
     if(empty_fields <= perfect_depth){
      cur_heur = -negamax_exact(board_stack+id,-TOTAL_FIELDS,TOTAL_FIELDS);
    }
    else{
      cur_heur = -negamax(board_stack+id,MIN_HEURISTIC,MAX_HEURISTIC,search_depth);
    }
    if(cur_heur > best_heur){
      best_heur = cur_heur;
      *res = board_stack[id];
    }
    std::cout << "move " << (child_count-id) << "/" << (child_count);
    std::cout << ": " << best_heur << std::endl;
  }
  
  
  struct timeval end;
  gettimeofday(&end,NULL);  
  double time_diff = (end.tv_sec + (end.tv_usec / 1000000.0)) - 
  (start.tv_sec + (start.tv_usec / 1000000.0));

  std::cout << nodes << " nodes in " << time_diff << " seconds: ";
  std::cout << (int)(nodes/(time_diff<0.000001 ? 1 : time_diff)) << " nodes / sec\n";
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
    *(b+1) = *b;
    (b+1)->turn = opponent((b+1)->turn);
    if(!(b+1)->has_moves()){
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

int bot_ali::negamax_win_draw_loss(board* b)
{
  /* this works because 
  WHITE win: res >= 2
  BLACK win: res <= -2 */
  int res = negamax_exact(b,-1,1); 
  return b->turn == WHITE ? res : -res;
}



void bot_ali::sort_boards(board *boards,int* heurs, int count)
{
  bool loop;
  do{
    loop = false;
    for(int i=count-1;i--;){
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
    
    int res = 0;
                      //            0   1   2   3    4   5   6   7   8   9
    static int open_loc_val[10] = {500,-71,-43,-33,-203,-23,-23, -9, -7,  3 };
    static int mid_loc_val[10] =  {500,-61,-37,-27,-203,-17,-17, -9, -7,  9 };
    static int end_loc_val[10] =  {400,-21,-21,-21, -53,-17,-17, -9, -7,  9 };
    
    int disc_count = (b->discs[WHITE] | b->discs[BLACK]).count();
    
    
    if(disc_count<=20){
      for(int i=0;i<10;i++){
        res += open_loc_val[i] * (b->discs[WHITE] & location_bitsets[i]).count();
        res -= open_loc_val[i] * (b->discs[BLACK] & location_bitsets[i]).count();
      }
    }
    if(disc_count>20 && disc_count<40){
      for(int i=0;i<10;i++){
        res += mid_loc_val[i] * (b->discs[WHITE] & location_bitsets[i]).count();
        res -= mid_loc_val[i] * (b->discs[BLACK] & location_bitsets[i]).count();
      }
    }
    if(disc_count>=40){
      for(int i=0;i<10;i++){
        res += end_loc_val[i] * (b->discs[WHITE] & location_bitsets[i]).count();
        res -= end_loc_val[i] * (b->discs[BLACK] & location_bitsets[i]).count();
      }
    }
    
    return res;
  
}

bot_ali::~bot_ali(){
  delete[] board_stack;
}