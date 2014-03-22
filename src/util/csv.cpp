#include "util/csv.hpp"

csv::csv(const std::string& _filename):
  file(_filename,std::fstream::in | std::fstream::out | std::fstream::app),
  filename(_filename)
{}

void csv::append_line(const std::vector< std::string >& x)
{
  for(unsigned i=0;i<x.size()-1;i++){
    file << x[i] << ',';
  }
  if(x.size()>0){
    file << x[x.size()-1];
  }
  file << '\n';
}

std::vector< std::vector< std::string > > csv::get_content()
{
  file.seekg(0, file.end);
  int filesize = file.tellg();
  file.seekg(0, file.beg);
  char* tmp = new char[filesize];
  file.read(tmp,filesize);
  tmp[filesize-1] = '\0'; //remove extra appended newline
  std::vector<std::string> lines;
  lines = str_explode(std::string(tmp),'\n');
  std::vector<std::vector<std::string> > res;
  for(const std::string s: lines){
    res.push_back(str_explode(s,','));
  } 
  return res;
}

void csv::set_content(std::vector< std::vector< std::string > >& x)
{
  file.close();
  file.open(filename,
            std::fstream::in | 
            std::fstream::out | 
            std::fstream::trunc
  );
  
  for(std::vector<std::string> vs: x){
    append_line(vs);
  }
}


