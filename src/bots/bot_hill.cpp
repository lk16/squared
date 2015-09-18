#include "bots/bot_hill.hpp"

REGISTER_BOT(hill);

bot_hill::bot_hill()
{
  set_name("hill");
}


void bot_hill::do_move(const board* b, board* res)
{
  int best_heur = MIN_HEURISTIC;
  int id = 0;
  board children[32];
  board* child_end = b->get_children(children);
  for(const board* child=children; child != child_end; ++child){
    board copy = *child;
    int current_heur = hill_climbing(&copy,get_search_depth(),get_perfect_depth()); 
    if(current_heur > best_heur){
      best_heur = current_heur;
      *res = *child;
    }
    output() << "move " << (id+1) << "/" << "TODO";
    output() << " (" << "TODO" << ')';
    output() << ": " << best_heur << '\n';
    ++id;
  }
}

int bot_hill::heuristic(const board* b){
  int res = b->count_valid_moves();
  int opp_move_count = b->count_opponent_moves();
  if(res==0 && opp_move_count==0){
    return EXACT_SCORE_FACTOR * b->get_disc_diff();
  }
  res -= opp_move_count;
  
  res += 3 *(
      (b->me & board::location[board::X_SQUARES]).count() 
      - (b->opp & board::location[board::X_SQUARES]).count()
  );
  
  return res;
}

int bot_hill::minimax(const board* b, int d,bool max)
{
  if(d==0){
    return (max ? 1 : -1) * heuristic(b);
  }
  bits64 valid_moves = b->get_valid_moves();
  if(valid_moves.none()){
    board copy = *b;
    copy.switch_turn();
    if(copy.get_valid_moves().any()){
      return minimax(&copy,d,!max);
    }
    else{
      return (max ? 1 : -1) * EXACT_SCORE_FACTOR * b->get_disc_diff();
    }
  }
  int best_heur;
  if(max){
    best_heur = MIN_HEURISTIC;
    while(valid_moves.any()){
      bits64 move_bit = valid_moves.first_bit();
      board copy = *b;
      copy.do_move(move_bit.only_bit_index());
      int current_heur = minimax(&copy,d-1,!max);
      if(current_heur > best_heur){
        best_heur = current_heur;
      }
      valid_moves ^= move_bit;
    }
  }
  else{
    best_heur = MAX_HEURISTIC;
    while(valid_moves.any()){
      bits64 move_bit = valid_moves.first_bit();
      board copy = *b;
      copy.do_move(move_bit.only_bit_index());
      int current_heur = minimax(&copy,d-1,!max);
      if(current_heur < best_heur){
        best_heur = current_heur;
      }
      valid_moves ^= move_bit;
    }
  }
  return best_heur;
}



int bot_hill::hill_climbing(board* b,int d,int pd){
  int turn = 0;
  while(64 - b->count_discs() > pd){
    board best_child;
    int best_heur = MIN_HEURISTIC;
    board children[32];
    board* child_end = b->get_children(children);
    for(const board* child = children;child != child_end;++child){
      board copy = *child;
      int heur = minimax(&copy,d,true);
      int ab = alphabeta(&copy,d,MIN_HEURISTIC,MAX_HEURISTIC);
      if(heur != ab){
        std::cout << "heur = " << heur << "\nab = " << ab << '\n';
        exit(1);
      }
      if(heur > best_heur){
        best_heur = heur;
        best_child = *child;
      }
    }
    *b = best_child;
    turn = 1 - turn;
  }
  int heur = minimax(b,pd,true);
  int ab = alphabeta(b,pd,MIN_HEURISTIC,MAX_HEURISTIC);
  if(heur != ab){
    std::cout << "heur = " << heur << "\nab = " << ab << '\n';
    exit(1);
  }
  return heur;  
}

int bot_hill::alphabeta(const board* b, int d, int alpha, int beta)
{
  if(d==0){
    return heuristic(b);
  }
  if(!b->has_valid_moves()){
    board copy = *b;
    copy.switch_turn();
    if(!copy.has_valid_moves()){
      return EXACT_SCORE_FACTOR * b->get_disc_diff();
    }
    else{
      return -alphabeta(&copy,d,-beta,-alpha);
    }
  }
  board children[32];
  board* child_end = b->get_children(children);
  for(const board* child=children; child!= child_end; ++child){
    int heur = -alphabeta(child,d-1,-beta,-alpha);
    if(heur > alpha){
      alpha = heur;
    }
//     if(alpha >= beta){
//       break;
//     }
  }
  return alpha;
}


void bot_hill::on_new_game(){
  return;
}

int bot_hill::heuristic(){
  return 0;
}

