#include "bot_ali.hpp"

#define SQUARED_BOT_ENABLE_OUTPUT 1
#include <string.h>

bot_ali::bot_ali(color _c, int _max_depth, int _max_endgame_depth):
  bot_base(_c, _max_depth, _max_endgame_depth)
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
}

void bot_ali::evaluate_depth_level(const board* boards, int* heurs, int count, int depth)
{
  int alpha = MIN_HEURISTIC;
  
  for(int id=count-1;id>=0;--id){
    heurs[id] = -alpha_beta(boards + id,alpha,MAX_HEURISTIC,depth);
    std::cout << heurs[id] << std::endl;
    if(heurs[id] > alpha){
      alpha = heurs[id];
    }
    std::cout << '.' << std::flush;
    
  }
}


void bot_ali::do_move(const board* b,board* res) 
{
  int alpha,child_count,best_move_id,depth;
  board children[TOTAL_FIELDS/2];  
  
  nodes = 0;
  alpha = MIN_HEURISTIC;
  best_move_id = -1;
  res->reset();
  
#if SQUARED_BOT_ENABLE_OUTPUT
  struct timeval start;
  gettimeofday(&start,NULL);
#endif  
  
  b->get_children(children,&child_count);
  assert(child_count>0);
  
  if(child_count==1){
    *res = children[0];
    return;
  } 
 
  depth = max_depth;
 
  if(TOTAL_FIELDS - (b->discs[BLACK] | b->discs[WHITE]).count() <= (unsigned)max_endgame_depth){
    //TODO separate endgame method
    // return something;
    depth = max_endgame_depth;
  }
  
  
  /*  Idea: big_child_stack will be broken down each run, 
      so we keep a copy (children) at all times to rebuild big_child_stack
      also it is used for the return value
  */
  board big_child_stack[(TOTAL_FIELDS/2)*15];
  int *heurs = new int[child_count];
  
  for(int d=1;d<=max_depth;d++){
    memcpy(&big_child_stack,children,child_count*sizeof(board));
#if SQUARED_BOT_ENABLE_OUTPUT
    std::cout << "Depth " << d << ": ";
    std::cout.flush();
#endif 
    evaluate_depth_level(big_child_stack,heurs,child_count,d);
    sort_boards(children,heurs,child_count);
#if SQUARED_BOT_ENABLE_OUTPUT
    std::cout << " " << heurs[child_count-1] << std::endl;
#endif  
  }

  *res = children[child_count-1];
  
  
#if SQUARED_BOT_ENABLE_OUTPUT
  struct timeval end;
  gettimeofday(&end,NULL);  
  double time_diff = (end.tv_sec + (end.tv_usec / 1000000.0)) - 
  (start.tv_sec + (start.tv_usec / 1000000.0));

  std::cout << nodes << " nodes in " << time_diff << " seconds: ";
  std::cout << (int)(nodes/(time_diff<0.000001 ? 1 : time_diff)) << " nodes / sec\n";
#endif
}




int bot_ali::alpha_beta(const board* b,int alpha, int beta,int depth_remaining)
{           
  nodes++; 
   
  if(b->test_game_ended()){
    return PERFECT_SCORE_FACTOR * (b->turn==WHITE ? 1 : -1) * b->get_disc_diff();    
  }
  if(depth_remaining==0){
    return (b->turn==WHITE ? 1 : -1) * heuristic(b);
  }  
  
  board children[32];
  int move_count;
  
  b->get_children(children,&move_count);
  if(move_count==0){
    board tmp(*b);
    tmp.turn = opponent(b->turn);
    return -alpha_beta(&tmp,-beta,-alpha,depth_remaining-1);
  }
  
  for(int id=0;id<move_count;id++){
    int value = -alpha_beta(children + id,-beta,-alpha,depth_remaining-1);
    if(value>=beta){
      return value;
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
    for(int i=0;i<count-1;i++){
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
  /*  0,1,2,3,3,2,1,0,
      1,4,5,6,6,5,4,1,
      2,5,7,8,8,7,5,2
      3,6,8,9,9,8,6,3,
      3,6,8,9,9,8,6,3,
      2,5,7,8,8,7,5,2
      1,4,5,6,6,5,4,1,
      0,1,2,3,3,2,1,0  */
  
  
  //                              0   1   2   3   4   5   6   7   8   9
  static int open_loc_val[10] = { 50, -8, -7, -6,-10, -3, -3, -4, -3, -2 };
  static int  mid_loc_val[10] = { 40, -4, -3, -2, -6, -4, -2, -2, -2,  0 };
  static int  end_loc_val[10] = { 20,  5,  7,  9, -2,  1,  0,  7,  1,  5 };
  
  int disc_count = (b->discs[WHITE] | b->discs[BLACK]).count();
  
  int res = 0;
  
  if(disc_count<=20){
    for(int i=0;i<10;i++){
      res += open_loc_val[i] * (b->discs[WHITE] & location_bitsets[i]).count();
      res -= open_loc_val[i] * (b->discs[BLACK] & location_bitsets[i]).count();
    }
  }
  if(disc_count>=15 && disc_count<45){
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

