#include "bots/bot_random.hpp"

REGISTER_BOT(random);

bot_random::bot_random()
{
}

void bot_random::disable_book()
{
}

void bot_random::disable_shell_output()
{
}

void bot_random::do_move(const board* in, board* out)
{
  board moves[32];
  board* moves_end = in->get_children(moves);
  
  *out = moves[rand() % (moves_end - moves)];
}

void bot_random::on_new_game()
{
}

bot_random::~bot_random()
{
}


