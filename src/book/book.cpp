#include "book/book.hpp"
#include "bots/bot_ali.hpp"

book_t::book_t():
  csv_file("")
{ 
}

book_t::book_t(const std::string& _filename):
  csv_file(_filename)
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

bool book_t::job_t::operator<(const book_t::job_t& j) const
{
  return priority < j.priority;
}

book_t::job_t::job_t(const board& _b, const book_t::value& _info, int book_stddev)
{
  b = _b;
  info = _info;
  assign_priority(book_stddev);
}

void book_t::job_t::assign_priority(int heur_stddev)
{
  (void)heur_stddev;
  
  if(info.heur % (2*EXACT_SCORE_FACTOR) == 0 && info.heur != 0){
    priority = -9999999;
    return;
  }
  
  if(info.depth < 12){
    priority = - b.count_discs();
    return;
  }
  
  
  priority = 0;
  priority -= 5 * (b.count_valid_moves() - b.count_opponent_moves());
  priority -= 7 * b.count_discs();
  priority -= 3 * abs(info.heur);
  priority -= 12 * info.depth;
}


int book_t::get_heur_stddev() const
{
  int total = 0.0;
  int count = 0;
  for(auto it: data){
    if(it.second.heur > -2000 && it.second.heur < 2000){
      count++;
      total += it.second.heur;
    }
  }
  
  const double avg = (double)total / count;
  
  double res = 0.0;
  
  for(auto it: data){
    if(it.second.heur > -2000 && it.second.heur < 2000){
      double diff = it.second.heur - avg;
      res += (diff*diff);
    }

  }
  
  res /= count;
  
  return sqrt<int>((int)res);
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
  if(!(bv.depth >= MIN_LEARN_DEPTH)){ 
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
  depth = from_str<int>(line[1]);
  best_move = from_str<int>(line[2]);
  if(line.size() > 3){
    heur = from_str<int>(line[3]);
  }
  else{
    heur = bot_base::NO_HEUR_AVAILABLE;
  }
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
  
  
  for(auto it: book_stats){
    std::cout << "Boards found at depth " << it.first << ": ";
    std::cout << it.second << std::endl;
  }
  std::cout << "Total boards in book: " << data.size() << std::endl;
}



void book_t::learn(bot_base* bot)
{
  //timeval last_reload,now;
  //gettimeofday(&last_reload,NULL);
  // WARNING reloading doesn't change job_queue
  
  bot->disable_shell_output();
  print_stats();
  
  std::priority_queue<job_t> job_queue;
  
  int book_stddev = get_heur_stddev();
  
  for(auto it: data){
    job_t job(board(it.first),it.second,book_stddev);
    job.info.depth++; //search one move deeper than already known
    job_queue.push(job);
  }
  
  while(true){
    job_t job = job_queue.top();
    value v = do_job(bot,&job);
    job_queue.pop();
    add(&job.b,&v);
    
    // update job and insert it into queue again
    job.info = v;
    job.info.depth++;
    job.info.heur = v.heur;
    job.assign_priority(book_stddev);
    job_queue.push(job);
    
    /*gettimeofday(&now,NULL);
    if(now.tv_sec - last_reload.tv_sec > RELOAD_INTERVAL){
      std::cout << "Reloading book" << std::endl;
      reload();
      print_stats();
      memcpy(&last_reload,&now,sizeof(timeval));
    }*/
  }
}

bool book_t::add(const board* b,const book_t::value* bv)
{
  
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
    && (b->count_discs() < ENTRY_MAX_DISCS)
    && (fixed_bv.depth >= MIN_LEARN_DEPTH)
    && ((it == data.end()) || (bv->depth > it->second.depth))
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


book_t::value book_t::do_job(bot_base* bot,const job_t* job){
  
  int depth = job->info.depth;
  std::cout << std::endl << std::endl;
  job->b.show();
  std::cout << "db string: " << job->b.to_database_string() << std::endl;
  std::cout << "depth: " << depth << std::endl;
  std::cout << "priority: " << job->priority << std::endl;
  std::cout << "last heur: " << job->info.heur << std::endl;
  bot->set_search_depth(depth,depth);
  bot->stats.start_timer();
  board after;
  bot->do_move(&job->b,&after);
  bot->stats.stop_timer();
  
  
  std::cout << "time: " << (int)bot->stats.get_seconds() << " s" << std::endl;
  std::cout << "nodes: " << big_number(bot->stats.get_nodes()) << std::endl; 
  std::cout << "speed: " << big_number(bot->stats.get_nodes_per_second()); 
  std::cout << "n/s" << std::endl;
  
  int move = job->b.get_move_index(&after);
  int heur = bot->get_last_move_heur();
  
  return value(move,depth,heur);
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
  value res(NOT_FOUND,0,0);
  std::string key = b->to_database_string();
  citer it = data.find(key);
  if(it == data.end() || (it->second.depth < min_depth)){
    return res;
  }
  res = it->second;
  bits64 move_bit = bits64_set[res.best_move];
  int rot = b->to_database_board().get_rotation(b);
  res.best_move = bits64_find_first(bits64_rotate(move_bit,rot));
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
