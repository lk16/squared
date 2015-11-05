#include "bot_base.hpp"


bot_base::bot_base():
  name("base"),
  output_stream(&std::cout)
{
  search_depth = -1;
  perfect_depth = -1;
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

void bot_base::stat_t::reset()
{
  nodes = 0ull;
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


void bot_base::on_new_game()
{
  return;
}
