#ifndef SQUARED_DEBUG_HEUR_ALGO_HPP
#define SQUARED_DEBUG_HEUR_ALGO_HPP

#include "game/board.hpp"
#include "game/util.hpp"
#include "bots/bot_ali.hpp"

// returns heur for WHITE
int minimax_dbg(const board* b,int depth);

// returns heur for WHITE
int alpha_beta_dbg(const board* b,int alpha,int beta,int depth);

// returns heur for b->turn
int negamax_dbg(const board* b,int alpha,int beta,int depth);

// returns heur for b->turn
int negamax_perfect_dbg(const board* b,int alpha,int beta);

#endif