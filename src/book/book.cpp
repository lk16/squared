#include "book/book.hpp"
#include "bots/bot_ali.hpp"

book_t::book_t():
  csv_file(""),
  ppool(nullptr)
{ 
}

book_t::book_t(const std::string& _filename):
  csv_file(_filename),
  ppool(nullptr)
{
  load();
}

std::string book_t::get_filename() const
{
  return csv_file.get_name();
}

void book_t::set_csv_file(const std::string& filename)
{
  csv_file.set_file(filename);
}

int book_t::job_priority(const board* b, int depth, int last_heur)
{
  
  if(depth < MIN_LEARN_DEPTH){
    return 99999999-depth;
  }
  if(last_heur%(2*EXACT_SCORE_FACTOR)==0 && last_heur!=0){
    return -99999999;
  }
  
  int res = 0;
  res += -7 * b->count_discs();
  res += -10 * depth;
  return res;
}




bool book_t::is_correct_entry(const std::string& bs,const book_t::value& bv) const
{
  board b(bs);
  
  int error = 0;
  
  if(!in_bounds<int>(bv.depth,0,60)){ 
    error = 1;
  }
  if(!in_bounds<int>(bv.best_move,0,63)){ 
    error = 2;
  }
  if(!(bs.length() == 32)){ 
    error = 3;
  }
  if(!(bv.depth >= MIN_ACCEPT_DEPTH)){ 
    error = 4;
  }
  if(!((b.me & b.opp) == 0ull)){ 
    error = 5;
  }
  if(!(((b.me | b.opp) & 0x0000001818000000) == 0x0000001818000000)){ 
    error = 6;
  }
  if(!(b.is_valid_move(bv.best_move))){ 
    error = 7;
  }
  if(!(b == b.to_database_board())){ 
    error = 8;
  }
  
  if(error == 0){
    return true;
  }
  std::cout << "Incorrect board data: error " << error << std::endl;
  return false;
}

book_t::value::value(const csv::line_t& line)
{
  depth = from_str<int>(line[COL_DEPTH]);
  best_move = from_str<int>(line[COL_BEST_MOVE]);
  heur = from_str<int>(line[COL_HEUR]);
}

book_t::value::value(int bm, int d,int h)
{
  best_move = bm;
  depth = d;
  heur = h;
}



void book_t::print_stats() const
{
  citer book_it;
  std::map<int,int> book_stats;
  
  for(book_it = data.begin(); book_it!=data.end(); book_it++){
    int n = book_it->second.depth;
    if(book_stats.find(n) == book_stats.end()){
      book_stats[n] = 1;
    }
    else{
      book_stats[n]++;
    }
  }
  
  
  std::cout << "Total boards in book: " << data.size() << std::endl;
  for(auto it: book_stats){
    std::cout << "Boards found at depth " << (it.first < 10 ? " " : "");
    std::cout << it.first << ": ";
    std::cout << it.second << std::endl;
  }
  
}





void book_t::learn(const std::string _bot_name,int threads)
{
  bot_name = _bot_name;
  
  
  print_stats();
  
  ppool = new priority_threadpool(threads);
  
  for(citer it=data.begin();it!=data.end();it++){
    board b(it->first);
    int depth = it->second.depth + 1;
    int prio = job_priority(&b,it->second.depth,it->second.heur);
    auto func = std::bind(&book_t::learn_job,this,b,depth);
    auto func2 = std::bind(&book_t::on_job_done,this);
    ppool->add_job(func,func2,prio);
  }
  
  ppool->start_workers();
  
  std::cout << "done adding all threads to thread pool, spinning forever" << std::endl;  
  while(!ppool->empty()){
    sleep(1);
  }
  
  std::cout << "somehow the threadpool became empty!\n";
  std::cout << "does the book file exist at all?\n";
  delete ppool;
}

void book_t::on_job_done()
{
  // this function does nothing at all on purpose
}

void book_t::learn_job(board b, int depth)
{
  depth = max(depth,MIN_LEARN_DEPTH);  
  
  bot_base* bot = bot_registration::bots()[bot_name]();
  bot->disable_book();
  bot->disable_shell_output();
  bot->set_search_depth(depth,depth);
  
  board out;
  bot->stats.start_timer();
  bot->do_move(&b,&out);
  bot->stats.stop_timer();
  
  int heur = bot->get_last_move_heur();
  int move = b.get_move_index(&out);
  value v(move,depth,heur);
  
  std::stringstream ss;
  ss << std::this_thread::get_id() << ": learned " << b.to_database_string();
  ss << " at depth " << depth << "\t" << big_number(bot->stats.get_nodes());
  ss << "n / " << (int)(bot->stats.get_seconds()) << " sec = ";
  ss << big_number(bot->stats.get_nodes_per_second()) << "n/sec\n";
  std::cout << ss.str();
  add(&b,&v);
  
  auto func = std::bind(&book_t::learn_job,this,b,depth+1);
  auto func2 = std::bind(&book_t::on_job_done,this);
  ppool->add_job(func,func2,job_priority(&b,depth+1,heur));
  
  delete bot;
}





bool book_t::add(const board* b,const book_t::value* bv)
{
  rw_lock_write_guard guard(data_lock);  
  // TODO heavily modify this
  board before_normalized = b->to_database_board();
  int rot = b->get_rotation(&before_normalized);
  board after = *b;
  after.do_move(bv->best_move);
  board after_normalized = after.rotate(rot);
  int move = before_normalized.get_move_index(&after_normalized);
  std::string str = b->to_database_string();
  citer it = data.find(str);
  
  book_t::value fixed_bv = *bv;
  fixed_bv.best_move = move;
  
  if(true 
    && (b->count_discs() <= ENTRY_MAX_DISCS)
    && (fixed_bv.depth >= MIN_ACCEPT_DEPTH)
    && ((it == data.end()) || (bv->depth > it->second.depth))
    && (bv->heur != bot_base::NO_HEUR_AVAILABLE)
  ){
    
    if(!is_correct_entry(str,fixed_bv)){
      std::cout << "WARNING: attempting to add invalid value to book!\n";
      return false;
    }
    
    csv::line_t book_line;
    book_line.push_back(str);
    fixed_bv.add_to_line(&book_line);
    
    csv_file.append_line(book_line);
    
    data[str] = fixed_bv;
    return true;
  }
  return false;
}

void book_t::value::add_to_line(csv::line_t* l) const
{
  l->push_back(to_str<int>(depth));
  l->push_back(to_str<int>(best_move));
  l->push_back(to_str<int>(heur));
}

void book_t::clean() const
{
  /* test if file exists, return if not */
  if(access(get_filename().c_str(), F_OK) == -1){
    std::cout << "file \"" << get_filename() << "\" not found." << std::endl;
    return;    
  }
    
  /* create backup file, to prevent losing data */
  std::rename(get_filename().c_str(),(get_filename() + ".bak").c_str());

  /* fill new file with data */
  csv new_book_file(get_filename());  
  
  for(auto it: data){
    if(is_correct_entry(it.first,it.second)){
      csv::line_t book_line;
      book_line.push_back(it.first);
      it.second.add_to_line(&book_line);
      new_book_file.append_line(book_line);
    }
  }
    
  /* remove backup file */
  std::remove((get_filename() + ".bak").c_str());
  
  std::cout << "Successfully cleaned \"" << get_filename() << "\"" << std::endl;
  
}

book_t::value book_t::lookup(const board* b,int min_depth)
{
  rw_lock_read_guard guard(data_lock);  
  value res(NOT_FOUND,0,0);
  std::string key = b->to_database_string();
  citer it = data.find(key);
  if((it!=data.end()) && (it->second.depth >= min_depth)){
    res = it->second;
    bits64 move_bit = bits64_set[res.best_move];
    int rot = b->to_database_board().get_rotation(b);
    res.best_move = bits64_find_first(bits64_rotate(move_bit,rot));
  }
  return res;
}

void book_t::reload()
{
  data.clear();
  csv_file.get_file()->seekg(std::ios_base::beg);
  load();
}

void book_t::load()
{
  int errors = 0;
  while(true){
    csv::line_t line = csv_file.get_line();
    if(csv_file.get_file()->fail()){
      break; 
    }
    if(line.size() < value::NEEDED_COLUMNS){
      errors++;
      continue;
    }
    book_t::value bv(line);
    
    if(is_correct_entry(line[0],bv)){
      data[line[0]] = bv;
    }
    else{
      errors++;
      continue;
    }
  }
  
  csv_file.get_file()->clear();
  
  if(errors > 0){
    std::cout << "Found " << errors;
    std::cout << " incorrect entries in ";
    std::cout << '\"' << get_filename() << '\"' << std::endl;
  }
}
