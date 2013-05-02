#include "bot_base.hpp"

bot_base::bot_base(color _c, int _max_depth, int _max_endgame_depth):
  max_depth(_max_depth),
  max_endgame_depth(_max_endgame_depth),
  look_ahead(3),
  c(_c),
  prev_move_time(std::time(NULL))
{
  init_hash_table();
}

void bot_base::do_move(const board* b,board* res) 
{
  int depth_limit,best_heur,tmp_heur,move_count;
  unsigned int id,best_move_id;
  board moves[TOTAL_FIELDS/2];
  std::string text;
  double time_diff;
  struct timeval start,end;
  
  
  nodes = 0;
  prev_move_time = std::time(NULL);
  gettimeofday(&start,NULL);
  best_heur = MIN_HEURISTIC;
  
  
  depth_limit = (b->max_moves_left() <= max_endgame_depth) ? (TOTAL_FIELDS-4) : max_depth;
  b->get_children(moves,&move_count);
  assert(move_count>0);
  
  best_move_id = -1;
  
  if(move_count==1){
    *res = moves[0];
    return;
  } 
  
  clear_hash_table();
  
  if(depth_limit - look_ahead >= 2){
    std::cout << "Sorting... "; 
    std::cout.flush();
    sort_boards(moves,move_count,look_ahead);
    std::cout << "Done\n";
  }
  
  clear_hash_table();
  
  for(id=0;(int)id<move_count;id++){
    tmp_heur = alpha_beta(moves + id,best_heur,MAX_HEURISTIC,depth_limit);
    if(tmp_heur > best_heur){
      best_heur = tmp_heur;
      text = "move " + tostr<int>(id+1) + "/" + tostr<int>(move_count) +": heuristic == ";
      text += tostr<int>(best_heur) + "\n";
      std::cout << text;
      /* TODO update statusbar msg */
      
      best_move_id = id;
      *res = moves[id];
    }
    else{
      text = "move " + tostr<int>(id+1) + "/" + tostr<int>(move_count) +": heuristic <= ";
      text += tostr<int>(best_heur) + "\n";
      std::cout << text;
      /* TODO update statusbar msg */
    }
  }

  /* bot can not prevent losing all discs -> just pick a move */
  if(best_heur == MIN_HEURISTIC){ 
    *res = moves[0];
  }

  gettimeofday(&end,NULL);  
  time_diff = (end.tv_sec + (end.tv_usec / 1000000.0)) - 
  (start.tv_sec + (start.tv_usec / 1000000.0));
  std::cout << nodes << " nodes in " << time_diff << " seconds: ";
  std::cout << (int)(nodes/(time_diff<0.000001 ? 1 : time_diff)) << " nodes / sec\n";
}




int bot_base::alpha_beta(const board* b,int alpha, int beta,int depth_remaining)
{
  board children[32];
  int move_count,id;
  board tmp;

#if SQUARED_BOT_USE_HASHTABLE  
  hash_table_entry *hash_entry;
  int hash;
#endif
  
  
  nodes++;   
  
  if(b->test_game_ended()){
    return (c==WHITE ? 1 : -1) * PERFECT_SCORE_FACTOR * b->get_disc_diff();
  }
  
  if(depth_remaining==0){
    return heuristic(b);
  }

#if SQUARED_BOT_USE_HASHTABLE
  if(max_depth - depth_remaining < 4){
    hash = b->hash() % HASH_TABLE_SIZE;
    hash_entry = hash_table[hash];
    while(hash_entry){
      if(*hash_entry->b == *b){
        return hash_entry->heur;
      }
      else{
        hash_entry = hash_entry->next;
      }
    }
  }
#endif
  
  
  
  
  b->get_children(children,&move_count);
  if(move_count==0){
    tmp = board(*b);
    tmp.turn = opponent(b->turn);
    return alpha_beta(&tmp,alpha,beta,depth_remaining);
  }
  
  if(b->turn == c){
    for(id=0;id<move_count;id++){
      alpha = max(alpha,alpha_beta(children + id,alpha,beta,depth_remaining-1));
      if(alpha>=beta){
        break;
      }
    }
#if SQUARED_BOT_USE_HASHTABLE
    if(max_depth - depth_remaining < 4){
      add_hash_table_entry(b,alpha);
    }
#endif
    return alpha; 
  }
  else{
    for(id=0;id<move_count;id++){
      beta = min(beta,alpha_beta(children + id,alpha,beta,depth_remaining-1));
      if(alpha>=beta){
        break;
      }
    }
#if SQUARED_BOT_USE_HASHTABLE
    if(max_depth - depth_remaining < 4){
      add_hash_table_entry(b,beta);
    }
#endif
    return beta;
  }
}

int bot_base::heuristic(const board* b)
{
  b = NULL; /* prevent compiler complaints */
  std::cout << "This is bot_base::heuristic(). Don't call me! Call my superclass instead! \n";
  CRASH;
  return 0;
}

void bot_base::sort_boards(board *moves,int move_count, int depth_limit)
{
  bool loop;
  int i,*heur,best_heur = MIN_HEURISTIC;
  
  heur = new int[move_count];
  
  for(i=0;i<move_count;i++){
    heur[i] = alpha_beta(moves + i,best_heur,MAX_HEURISTIC,depth_limit);
    if(heur[i] > best_heur){
      best_heur = heur[i];
    }
  }
  do{
    loop = false;
    for(i=0;i<move_count-1;i++){
      if(heur[i] < heur[i+1]){
        std::swap(heur[i],heur[i+1]);
        std::swap(moves[i],moves[i+1]);
        loop = true;
      }
      assert(heur[i] >= heur[i+1]);
    }
  }while(loop);
  
  delete[] heur;
}

int bot_base::get_max_depth() const
{
  return max_depth;
}

void bot_base::clear_hash_table()
{
  int i;
  hash_table_entry *hte,*tmp;
  
  for(i=0;i<HASH_TABLE_SIZE;i++){
    hte = hash_table[i];
    while(hte != NULL){
      tmp = hte->next;
      delete hte->b;
      delete hte;
      hte = tmp;
    }
    hash_table[i] = NULL;
  }
  
}
  
void bot_base::init_hash_table()
{
  int i;
  
  for(i=0;i<HASH_TABLE_SIZE;i++){
    hash_table[i] = NULL;
  }
}

void bot_base::add_hash_table_entry(const board* b, int heur)
{
  int bucket;
  hash_table_entry* hte;
  
  bucket = b->hash() % HASH_TABLE_SIZE;
  if(hash_table[bucket] == NULL){
    hash_table[bucket] = new hash_table_entry(NULL,new board(*b),heur);
  }
  else{
    hte = hash_table[bucket];
    while(hte->next != NULL){
      hte = hte->next;
    }
    hte->next = new hash_table_entry(NULL,new board(*b),heur);
  }
}

bot_base::~bot_base()
{
  clear_hash_table();
}
