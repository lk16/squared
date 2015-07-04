#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "util/const.hpp"


template<class T>
class arg_parser_base{
  
protected:
 
  const char **current_arg,**end_arg;
  

  bool error_flag;
  
  // check whether there are enough remaining arguments
  // --set-value value -> use n=1
  // -lb 10 16 -> use n=2
  bool has_enough_args(int n);
  
  const char* get_arg(int n) const;

  // maps arguments to a function that modifies the argument state
  // that function should return how many arguments it has used
  // or IGNORE_OTHER_ARGS or ERROR when appropriate
  typedef std::map<std::string,int(T::*)()> func_map_t;
  func_map_t func_map;
  
  
public:
  
  arg_parser_base(int _argc,const char** _argv);
  
  //returns false on errors
  bool parse();
  
};

template<class T>
inline arg_parser_base<T>::arg_parser_base(int _argc,const char** _argv):
  current_arg(_argv+1),
  end_arg(_argv+_argc),
  error_flag(false)
{
}



template<class T>
inline bool arg_parser_base<T>::parse()
{
  error_flag = false;
  typename func_map_t::const_iterator it;
  while(!error_flag && current_arg<end_arg){
    it = func_map.find(std::string(*current_arg));
    if(it != func_map.end()){
      int diff = (((T*)(this))->*(it->second))();
      if(diff == PARSING_ERROR){
        error_flag = true;
      }
      else if(diff == PARSING_IGNORE_OTHER_ARGS){
        break;
      }
      else{
        current_arg += diff;
      }
    }
    else{
      error_flag = true;
    }
  }
  if(error_flag){
    std::cout << "Invalid argument syntax. Try the -h flag for help\n";
  }
  return !error_flag;
}


template<class T>
inline bool arg_parser_base<T>::has_enough_args(int n)
{
  if(current_arg + n >= end_arg){
    error_flag = true;
    return false;
  }  
  return true;
}

template<class T>
inline const char* arg_parser_base<T>::get_arg(int n) const
{
  return *(current_arg + n);
}