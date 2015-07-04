#include "bot_base.hpp"


bot_base::bot_base():
  name("base"),
  output_stream(&std::cout),
  book(nullptr)
{
}

void bot_base::set_last_move_heur(int heur)
{
  last_move_heur = heur;
}


int bot_base::get_last_move_heur() const
{
  return last_move_heur;
}

int bot_base::rough_prediction(const board* b) const
{
  (void)b;
  std::cout << "warning: bot_" << get_name() << " did not implement ";
  std::cout << " rough_prediction()\n";
  return 0;
}



int bot_base::get_search_depth() const
{
  return search_depth; 
}

int bot_base::get_perfect_depth() const
{
  return perfect_depth;
}

void bot_base::set_search_depth(int _search_depth, int _perfect_depth)
{       
  search_depth = _search_depth;
  perfect_depth = _perfect_depth;
}

void bot_base::stat_t::start_timer()
{
  nodes = 0ull;
  gettimeofday(&start_time,NULL);
}

void bot_base::stat_t::stop_timer()
{
  gettimeofday(&stop_time,NULL);
}

double bot_base::stat_t::get_seconds() const
{
  return (stop_time.tv_sec + (stop_time.tv_usec / 1000000.0)) - 
  (start_time.tv_sec + (start_time.tv_usec / 1000000.0)); 
}

unsigned long long bot_base::stat_t::get_nodes_per_second() const
{
  if(get_seconds() < 0.00000001){
    return 0ull;
  }
  return (unsigned long long)(nodes / get_seconds());
}

bot_base::stat_t::stat_t()
{
  nodes = 0;
}

long long unsigned int bot_base::stat_t::get_nodes() const
{
  return nodes;
}

void bot_base::stat_t::inc_nodes()
{
  nodes++;
}

void bot_base::disable_book()
{
  if(book){
    delete book;
    book = nullptr;
  }
}

void bot_base::disable_shell_output()
{
  output_stream = new dummystream;
}

void bot_base::set_name(const std::string& _name)
{
  name = _name;
}

std::string bot_base::get_name() const
{
  return name;
}

std::ostream& bot_base::output()
{
  return *output_stream;
}

bool bot_base::get_use_book() const
{
  return book!=nullptr;
}

void bot_base::on_new_game()
{
  if(get_use_book()){
    book->reload();
  }
}
