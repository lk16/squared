#include "util/csv.hpp"

csv::csv(const std::string& _filename):
  filename(_filename),
  line_buff_size(10240),
  line_buff(new char[line_buff_size]),
  file(_filename,std::fstream::in | std::fstream::out | std::fstream::app)
{}

csv::~csv()
{
  delete[] line_buff;
}


void csv::append_line(const line_t& line)
{
  for(unsigned i=0;i<line.size()-1;i++){
    file << line[i] << ',';
  }
  if(line.size()>0){
    file << line[line.size()-1];
  }
  file << '\n';
  file.flush();
}

csv::content_t csv::get_content()
{
  content_t res;
  while(file.good()){
    res.push_back(get_line());
  }
  return res;
}

void csv::set_content(const content_t& content)
{
  file.close();
  file.open(filename,
            std::fstream::in | 
            std::fstream::out | 
            std::fstream::trunc
  );
  
  for(line_t line: content){
    append_line(line);
  }
}

csv::line_t csv::get_line()
{
  file.getline(line_buff,line_buff_size);
  return str_explode(std::string(line_buff),',');
}

const std::fstream* csv::get_file() const
{
  return &file;
}
