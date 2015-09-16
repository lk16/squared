#include "bots/bot_hill.hpp"

REGISTER_BOT(hill);

bot_hill::bot_hill()
{
  set_name("hill");
}


void bot_hill::do_move(const board* b, board* res)
{
  bits64 valid_moves = b->get_valid_moves();
  if(valid_moves.count() == 1){
    *res = *b;
    res->do_move(valid_moves.only_bit_index());
    return;
  }
  int best_heur = MIN_HEURISTIC;
  board best_child;
  int id = 0;
  int child_count = valid_moves.count();
  while(valid_moves.any()){
    bits64 move = valid_moves.first_bit();
    int move_index = move.only_bit_index();
    board copy = *b;
    copy.do_move(move_index);
    board copy2 = copy;
    int current_heur = hill_climbing(&copy,get_search_depth(),get_perfect_depth());
    if(current_heur > best_heur){
      best_heur = current_heur;
      best_child = copy2;
    }
    output() << "move " << (id+1) << "/" << (child_count);
    output() << " (" << board::index_to_position(move_index) << ')';
    output() << ": " << best_heur << '\n';
    valid_moves.reset(move_index);
    ++id;
  }
  *res = best_child;
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

int bot_hill::alphabeta(const board* b,int d,int alpha,int beta){
  if(d==0){
    return -heuristic(b);
  }
  bits64 valid_moves = b->get_valid_moves();
  if(valid_moves.none()){
    board copy = *b;
    copy.switch_turn();
    if(copy.get_valid_moves().any()){
      return -alphabeta(&copy,d,-beta,-alpha);
    }
    else{
      return b->get_disc_diff();
    }
  }
  
  while(valid_moves.any()){
    bits64 move = valid_moves.first_bit();
    int move_index = move.only_bit_index();
    board copy = *b;
    int current_heur = -alphabeta(&copy,d-1,-beta,-alpha);
    if(current_heur > alpha){
      alpha = current_heur;
    }
    if(alpha >= beta){
      return beta;
    }
    valid_moves.reset(move_index);
  }
  return alpha;
}

int bot_hill::hill_climbing(board* b,int d,int pd){
  while(64 - b->count_discs() > pd){
    bits64 valid_moves = b->get_valid_moves();
    if(valid_moves.none()){
      b->switch_turn();
      if(b->get_valid_moves().any()){
        continue;
      }
      else{
        return b->get_disc_diff();
      }
    }
    int best_move = -1;
    int best_heur = MIN_HEURISTIC;
    while(valid_moves.any()){
      bits64 move_bit = valid_moves.first_bit();
      int move_index = move_bit.only_bit_index();
      board copy = *b;
      int heur = -alphabeta(&copy,d,MIN_HEURISTIC,-best_heur);
      if(heur > best_heur){
        best_heur = heur;
        best_move = move_index;
      }
      valid_moves ^= move_bit;
    }
    b->do_move(best_move);
  }
  return -alphabeta(b,pd,MIN_HEURISTIC,MAX_HEURISTIC);
}

void bot_hill::on_new_game(){
  
}

int bot_hill::heuristic(){
  return 0;
}

