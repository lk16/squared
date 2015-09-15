#include "book/book.hpp"
#include "bots/bot_ali.hpp"

book_t::book_t():
  csv_file("")
{ 
}

book_t::book_t(const std::string& filename):
  csv_file(filename)
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

bool book_t::is_suitable_entry(const entry& e) const
{
  board b(e.first);
  
  return true
    && (b.count_discs() <= ENTRY_MAX_DISCS)
    && (e.second.depth >= MIN_ACCEPT_DEPTH)
    && (e.second.heur != bot_base::NO_HEUR_AVAILABLE); 
}



bool book_t::is_correct_entry(const entry& e) const
{
  board b(e.first);
  
  int error = 0;
  
  if(out_bounds<int>(e.second.depth,0,60)){
    error = 1;
  }
  if(out_bounds<int>(e.second.best_move,0,63)){ 
    error = 2;
  }
  if(e.first.length() != 32){ 
    error = 3;
  }
  if(e.second.depth < MIN_ACCEPT_DEPTH){ 
    error = 4;
  }
  if((b.me & b.opp).any()){ 
    error = 5;
  }
  if(((b.me | b.opp) & bits64(0x0000001818000000)) != bits64(0x0000001818000000)){ 
    error = 6;
  }
  if(!b.is_valid_move(e.second.best_move)){ 
    error = 7;
  }
  if(b != b.to_database_board()){ 
    error = 8;
  }
  if(error != 0){
    std::cout << "Incorrect board data: error " << error << '\n';
  }
  return error == 0;
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
  std::map<int,int> book_stats;
  
  for(auto book_it: container){
    int n = book_it.second.depth;
    if(book_stats.count(n) == 0){
      book_stats[n] = 1;
    }
    else{
      book_stats[n]++;
    }
  }
  
  
  std::cout << "Total boards in book: " << container.size() << '\n';
  for(auto it: book_stats){
    std::cout << "Boards found at depth " << (it.first < 10 ? " " : "");
    std::cout << it.first << ": ";
    std::cout << it.second << '\n';
  }
  
}

void book_t::learn(const std::string& bot_name,unsigned threads)
{
  print_stats();
  
  std::priority_queue<learn_job> pq;
  std::mutex mutex;
  
  for(auto it: container){
    board b(it.first);
    int depth = it.second.depth + 1;
    pq.push(learn_job(b,depth));
  }
  
  std::cout << "done adding all threads to thread pool, spinning forever" << '\n'; 
  
  std::vector<std::thread*> thread_vec;
  for(unsigned i=0;i<threads;++i){
    thread_vec.push_back(new std::thread(&book_t::learn_thread,this,bot_name,&pq,&mutex));
  }
  
  for(auto th: thread_vec){
    th->join();
  }
   
  while(true);
}

void book_t::learn_thread(const std::string& bot_name,std::priority_queue<book_t::learn_job>* pq, std::mutex* mutex)
{
  bot_base* bot = bot_registration::bots()[bot_name]();
  learn_job job;
  while(true){
    mutex->lock();
    job = pq->top();
    pq->pop();
    mutex->unlock();
    learn_execute_job(bot,&job);
  }
}

void book_t::learn_execute_job(bot_base* bot,learn_job* job){
  int depth = max(job->depth,MIN_LEARN_DEPTH);  
  board b = job->b;
  
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


}





bool book_t::add(const board* b,const book_t::value* bv)
{  
  rw_lock_write_guard guard(container_lock);  
  // TODO heavily modify this
  board before_normalized = b->to_database_board();
  int rot = b->get_rotation(&before_normalized);
  board after = *b;
  after.do_move(bv->best_move);
  board after_normalized = after.rotate(rot);
  int move = before_normalized.get_move_index(&after_normalized);
  std::string str = b->to_database_string();
  iter it = container.find(str);
  
  book_t::value fixed_bv = *bv;
  fixed_bv.best_move = move;
  
  if(!is_suitable_entry(*it)){
    return false;  
  }
  
  if(!is_correct_entry(*it)){
    std::cout << "WARNING: attempting to add invalid value to book!\n";
    return false;
  }
    
  csv::line_t book_line;
  book_line.push_back(str);
  fixed_bv.add_to_line(&book_line);
  
  csv_file.append_line(book_line);
  
  container[str] = fixed_bv;
  return true;
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
    std::cout << "file \"" << get_filename() << "\" not found." << '\n';
    return;    
  }
    
  /* create backup file, to prevent losing data */
  std::rename(get_filename().c_str(),(get_filename() + ".bak").c_str());

  /* fill new file with data */
  csv new_book_file(get_filename());  
  
  for(auto it: container){
    if(is_correct_entry(it) && is_suitable_entry(it)){
      csv::line_t book_line;
      book_line.push_back(it.first);
      it.second.add_to_line(&book_line);
      new_book_file.append_line(book_line);
    }
  }
    
  /* remove backup file */
  std::remove((get_filename() + ".bak").c_str());
  
  std::cout << "Successfully cleaned \"" << get_filename() << "\"" << '\n';
  
}

book_t::value book_t::lookup(const board* b,int min_depth)
{
  rw_lock_read_guard guard(container_lock);  
  value res(NOT_FOUND,0,0);
  std::string key = b->to_database_string();
  citer it = container.find(key);
  if((it!=container.end()) && (it->second.depth >= min_depth)){
    res = it->second;
    bits64 move_bit = bits64().set(res.best_move);
    int rot = b->to_database_board().get_rotation(b);
    res.best_move = move_bit.rotate(rot).only_bit_index();
  }
  return res;
}

void book_t::reload()
{
  container.clear();
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
    
    book_t::entry e(line[0],book_t::value(line));
    
    if(is_correct_entry(e)){
      container.insert(e);
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
    std::cout << '\"' << get_filename() << "\"\n";
  }
}
