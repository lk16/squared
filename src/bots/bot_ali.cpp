#include "bot_ali.hpp"
#include <../../../src/fador-mineserver-7a12610/include/tools.h>

bot_ali::bot_ali(color _c, int sd, int wdl, int pd):
  bot_base(_c,sd,wdl,pd),
  table(1023)
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
  
  
  child_count = b->get_children(children) - children;
  
  assert(child_count != 0);
  

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

#if BOT_ALI_EXPAND_CHILDREN 

  std::vector<std::pair<board,int> > descendants;
  
  if(search_depth>BOT_ALI_MIN_SEARCH_DEPTH_TO_SORT){
    int heurs[32];
    for(int i=0;i<child_count;i++){
      inspected = children[i];
      heurs[i] = -negamax(MIN_HEURISTIC,MAX_HEURISTIC,search_depth-4);
    }
    sort_boards(children,heurs,child_count);
  }
  for(int id=0;id<(int)descendants.size();++id){
    inspected = descendants[id].first;
    
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
      best_id = descendants[id].second;
    }
    
    if(shell_output){
      if(((id+1)%100==0) || (id+1)==(int)descendants.size()){
        std::cout << "move " << (id+1) << "/" << (descendants.size());
        std::cout << ": " << best_heur << std::endl;
      }
    }
  }
  
#else

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
  
#endif
  
#if BOT_ALI_USE_HASHTABLE  
  //std::cout << "table.size() = " << table.size() << '\n';
  table.clear();
#endif
  
  
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
  
#if BOT_ALI_USE_HASHTABLE  
  bool valid_hash_table_depth = (
    (search_depth - depth_remaining) >= BOT_ALI_MIN_HASH_TABLE_DEPTH && 
    (search_depth - depth_remaining) <= BOT_ALI_MAX_HASH_TABLE_DEPTH
  );
  if(valid_hash_table_depth){
    std::unordered_map<board,int>::const_iterator it = table.find(inspected);
    if(it != table.end()){
      return it->second;
    }
  }
#endif
  
  
  
  
  int child_count=0;
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
#if BOT_ALI_USE_HASHTABLE  
        if(valid_hash_table_depth){
          table.insert(std::make_pair<board,int>(inspected,beta));
        }
        return beta;
#endif
      }
      if(value >= alpha){
        alpha = value;
      }
      child_count++;
    }
    possible_moves.reset(move);
  }
  if(child_count!=0){
#if BOT_ALI_USE_HASHTABLE  
    if(valid_hash_table_depth){
      table.insert(std::make_pair<board,int>(inspected,alpha));
    }
#endif
    return alpha;
  }
  else{
    board tmp[32];
    int heur;
    
    inspected.switch_turn();
    
    if(inspected.get_children(tmp) != tmp){
      heur = -negamax(-beta,-alpha,depth_remaining-1);
      inspected.switch_turn();      
#if BOT_ALI_USE_HASHTABLE  
      if(valid_hash_table_depth){
        table.insert(std::make_pair<board,int>(inspected,heur));
      }
#endif
      return heur;
    }
    else{
      // at this point inspected.turn is switched
      inspected.switch_turn();
      heur = EXACT_SCORE_FACTOR * inspected.get_disc_diff();
      if(inspected.turn==BLACK){
        heur = -heur;
      }
#if BOT_ALI_USE_HASHTABLE  
      if(valid_hash_table_depth){
        table.insert(std::make_pair<board,int>(inspected,heur));
      }
#endif
      return heur;
    }
  }
  
}




int bot_ali::negamax_exact(int alpha, int beta)
{
  
  nodes++;
  
  
  int max_moves_left = 64 - (inspected.discs[WHITE] | inspected.discs[BLACK]).count();
  
  bool valid_hash_table_depth = (max_moves_left > 14);
  if(valid_hash_table_depth){
    std::unordered_map<board,int>::const_iterator it = table.find(inspected);
    if(it != table.end()){
      return it->second;
    }
  }
  
  
  int child_count=0;
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
        if(valid_hash_table_depth){
          table.insert(std::make_pair<board,int>(inspected,beta));
        }
        return beta;
      }
      if(value >= alpha){
        alpha = value;
      }
      child_count++;
    }
    possible_moves.reset(move);
  }
  
  if(child_count!=0){
    return alpha;
  }
  else{
    if((inspected.discs[BLACK] | inspected.discs[WHITE]).count()!=64){
      board tmp[32];
      
      inspected.switch_turn();
     
      int heur;
      if(inspected.get_children(tmp) != tmp){
        heur = -negamax_exact(-beta,-alpha);
        inspected.switch_turn();
        if(valid_hash_table_depth){
          table.insert(std::make_pair<board,int>(inspected,heur));
        }
        return heur;
      }
      inspected.switch_turn();
    }
    int tmp =  (inspected.turn==WHITE ? inspected.get_disc_diff() :
      -inspected.get_disc_diff());
      
    if(valid_hash_table_depth){
      table.insert(std::make_pair<board,int>(inspected,tmp));
    }
    return tmp;
  }
}

int bot_ali::hill_climbing(board* b,int look_ahead, int depth)
{
  
  if(depth == 0){
    int heur = heuristic();
    if(inspected.turn == BLACK){
      heur = -heur;
    }
    return heur;
  }
  
  board children[32],*children_end;
  children_end = b->get_children(children);
  
  board best;
  int best_heur;
  
  best_heur = (b->turn == WHITE) ? MAX_HEURISTIC : MIN_HEURISTIC; 
  
  
  for(board* it=children;it != children_end; it++){
    inspected = *it;
    int heur = negamax(MIN_HEURISTIC,MAX_HEURISTIC,look_ahead);
    if((b->turn == WHITE) && (heur < best_heur)){
      best_heur = heur;
      best = *it;
    }
    if((b->turn == BLACK) && (heur < best_heur)){
      best_heur = heur;
      best = *it;
    }
  }
  
  return hill_climbing(&best,look_ahead,depth-1);
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

void bot_ali::sort_board_int_pairs(std::vector< std::pair< board, int > >& boards)
{
  int* heurs = new int[boards.size()];
  
  for(int i=0;i<(int)boards.size();i++){
    inspected = boards[i].first;
    heurs[i] = heuristic();
  }
  
  
  
  
  bool loop;
  do{
    loop = false;
    for(int i=1;i<(int)boards.size();++i){
      if(heurs[i-1] < heurs[i]){
        std::swap(heurs[i-1],heurs[i]);
        std::swap(boards[i-1],boards[i]);
        loop = true;
      }
    }
  }while(loop);
  
  delete heurs;
  
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
      347, // 0
     - 39, // 1
     - 23, // 2
     - 40, // 3
     -119, // 4
     - 35, // 5
     - 33, // 6
     - 9, // 7
     - 7, // 8
     - 5 // 9
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