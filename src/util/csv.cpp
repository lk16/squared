#include "util/csv.hpp"

csv::csv(const std::string& _filename):
  filename(_filename),
  line_buff_size(10240),
  line_buff(new char[line_buff_size]),
  file(_filename,std::fstream::in | std::fstream::out | std::fstream::app),
  read_lines(0),
  written_lines(0)
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
  written_lines++;
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
  read_lines++;
  return str_explode(std::string(line_buff),',');
}

std::fstream* csv::get_file()
{
  return &file;
}

int csv::get_read_lines() const
{
  return read_lines;
}

int csv::get_written_lines() const
{
  return written_lines;
}

std::string csv::get_name() const
{
  return filename;
}
