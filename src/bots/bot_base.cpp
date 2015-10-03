#include "bot_base.hpp"


bot_base::bot_base():
  name("base"),
  output_stream(&std::cout)
{
  search_depth = -1;
  perfect_depth = -1;
  book = nullptr;
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


void bot_base::open_book()
{
  if(book || (get_name() == "base")){
    return;
  }
  book = new SQLite::Database("./book/openings.db3",SQLITE_OPEN_READWRITE);
  std::string query = 
  "CREATE TABLE IF NOT EXISTS \"bot_" + get_name() + "\" ("
  "`board` BLOB NOT NULL,"
  "`depth` INTEGER NOT NULL,"
  "`heuristic` INTEGER NOT NULL,"
  "`pv` INTEGER NOT NULL,"
  "PRIMARY KEY(board)"
  ")"; 
  try{
    book->exec(query);
  }
  catch(const SQLite::Exception& e){
    std::cerr << e.what() << '\n';
  }
}

void bot_base::add_to_book(const board* b, int depth, int heuristic, int pv)
{
  if(!book){
    return;
  }
  std::string query_str = "INSERT INTO bot_" + get_name() + " VALUES(?,?,?,?)";
  SQLite::Statement query(*book,query_str);
  query.bind(1,b,sizeof(board));
  query.bind(2,depth);
  query.bind(3,heuristic);
  query.bind(4,pv);
  try{
    query.exec();
  }
  catch(const SQLite::Exception& e){
    std::cerr << e.what() << '\n';
  }
}

bool bot_base::lookup_book(const board* b, int depth, int* heuristic, int* pv)
{
  if(!book){
    return false;
  }
  std::string query_str = "SELECT * FROM bot_" + get_name() + " WHERE board = ?";
  SQLite::Statement query(*book,query_str);
  query.bind(1,b,sizeof(board));
  try{
    query.exec();
    if(query.getColumn(1).getInt() < depth){
      return false;
    }
    *heuristic = query.getColumn(2).getInt();
    *pv = query.getColumn(3).getInt();
    return true;
  }
  catch(const SQLite::Exception& e){
    std::cerr << e.what() << '\n';
  }
  return false;
}


bot_base::~bot_base()
{
  if(book){
    delete book;
  }
}
