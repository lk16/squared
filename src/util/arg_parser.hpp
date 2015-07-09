#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "util/const.hpp"

/* HOW TO USE:
 * create a class x_parser (where x could be the program name)
 * that inherits from arg_parser_base<x_parser>
 * 
 * x_parser should have modifier functions with this signature:
 * int x_parser::handle_some_flag()
 */

template<class T>
class arg_parser_base{
  
protected:
 
  const char **current_arg,**end_arg;
  

  bool error_flag;
  
  // check whether there are enough remaining sub arguments
  // --set-value value -> use n=1
  // -lb 10 16 -> use n=2
  bool has_enough_subargs(int n);
  
  const char* get_subarg(int n) const;

private:
  
  struct modifier{
    int(T::*func)();
    std::string description,sub_args;
  };
  
  // maps arguments to a function that modifies the argument state
  // that function should return how many arguments it has used
  // or IGNORE_OTHER_ARGS or ERROR when appropriate
  std::map<std::string,modifier> func_map;
  
public:
  
  arg_parser_base(int _argc,const char** _argv);
  
  void add_modifier(const std::string& arg,int(T::*func)(),const std::string& sub_args,const std::string& description);
  
  void duplicate_modifier(const std::string& arg,const std::string& original);
  
  void print_general_help() const;
  void print_specific_help(const std::string& arg) const;
  
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
inline void arg_parser_base<T>::add_modifier(const std::string& arg, int(T::*func)(),const std::string& sub_args,const std::string& description)
{
  if(arg == "-h" || arg == "--help"){
    std::cerr << "WARNING from arg_parser_base::add_modifier(): you can not add a help function manually.\n";
    return;
  }
  modifier mod;
  mod.func = func;
  mod.description = description;
  mod.sub_args = sub_args;
  func_map[arg] = mod;
}

template<class T>
inline void arg_parser_base<T>::duplicate_modifier(const std::string& arg, const std::string& original)
{
  if(func_map.find(original) == func_map.end()){
    std::cerr << "WARNING from arg_parser_base::duplicate_modifier(): \"" << original << "\" was not found in func_map.\n";
    return;
  }
  func_map[arg] = func_map[original];
}



template<class T>
inline bool arg_parser_base<T>::parse()
{
  error_flag = false;
  while(current_arg<end_arg){
    std::string arg = *current_arg;
    if(arg == "-h" || arg == "--help"){
      if(has_enough_subargs(1)){
        print_specific_help(get_subarg(1));
      }
      else{
        print_general_help();
      }
      exit(0);
    }
    auto it = func_map.find(arg);
    if(it == func_map.end()){
      error_flag = true;
      break;
    }
    
    int diff = (((T*)(this))->*(it->second.func))();
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
  if(error_flag){
    std::cout << "Argument error. For help use: -h or --help, optionally supplied with a flag you want help with.\n";
  }
  return !error_flag;
}


template<class T>
inline bool arg_parser_base<T>::has_enough_subargs(int n)
{
  if(current_arg + n >= end_arg){
    error_flag = true;
    return false;
  }  
  return true;
}

template<class T>
inline const char* arg_parser_base<T>::get_subarg(int n) const
{
  return *(current_arg + n);
}

template<class T>
inline void arg_parser_base<T>::print_specific_help(const std::string& arg) const
{
  auto it = func_map.find(arg);  
  if(it == func_map.end()){
    std::cout << "Flag \"" << arg << "\" does not exist.\n";
    return;
  }
  std::cout << arg << ' ' << it->second.sub_args << '\n';
  std::cout << it->second.description << '\n';
}

template<class T>
inline void arg_parser_base<T>::print_general_help() const
{
  for(auto it: func_map){
    print_specific_help(it.first);
    std::cout << '\n';
  }
}

