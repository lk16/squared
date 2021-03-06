#include "bots/bot_random.hpp"

REGISTER_BOT(random);

bot_random::bot_random()
{
  set_name("random");
}

void bot_random::do_move(const board* in, board* out)
{
  board moves[32];
  board* moves_end = in->get_children(moves);
  
  *out = moves[rand() % (moves_end - moves)];
  output() << "bot_" << get_name() << " picked a move.\n";
}

void bot_random::on_new_game()
{
}



