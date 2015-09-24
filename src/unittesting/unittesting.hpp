#pragma once

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <set>

#include "game/board.hpp"
#include "util/math.hpp"

#define add_to_fun_vec(vec,f) \
    vec.push_back(std::make_pair(squared_unittest::f,#f))

class progress_bar{
  static const int bar_size = 100;
  int bar_printed_chars = 0;
  int total_steps;
  int steps_done;
  
  
public:
  progress_bar(int n,const std::string& func_name){ 
    std::cout << "\n" << std::setw(40) << func_name << "  ";
    std::cout.flush();
    total_steps = n;
    steps_done = 0;
    bar_printed_chars = 0;
  }
  
  void step(){
    ++steps_done;
    const int needed = (int)((((float)steps_done)/total_steps)*bar_size);
    while(needed != bar_printed_chars){
      std::cout << '.';
      ++bar_printed_chars;
    }
    std::cout.flush();
  }
  
  ~progress_bar(){
    while(bar_printed_chars < bar_size){
      std::cout << '.';
      ++bar_printed_chars;
    }
    std::cout.flush();
  }
  
};

class squared_unittest{
  
  static const int N = 100;
  
  static void announce(const std::string& name);
  static void TODO();
  
  
  static void test_bits64_operator_equals(const bits64* x,const bits64* y);
  static void test_bits64_operator_unequals(const bits64* x,const bits64* y);
  static void test_bits64_compound_assign_or(const bits64* x,const bits64* y);
  static void test_bits64_compound_assign_and(const bits64* x,const bits64* y);
  static void test_bits64_compound_assign_xor(const bits64* x,const bits64* y);
  static void test_bits64_compound_assign_shift(const bits64* x);
  static void test_bits64_operator_tilde(const bits64* x);
  static void test_bits64_operator_or(const bits64* x,const bits64* y);
  static void test_bits64_operator_and(const bits64* x,const bits64* y);
  static void test_bits64_operator_xor(const bits64* x,const bits64* y);
  static void test_bits64_operator_bool(const bits64* x);
  static void test_bits64_reset_all(const bits64* x);
  static void test_bits64_reset_before(const bits64* x);
  static void test_bits64_reset_after(const bits64* x);
  static void test_bits64_reset(const bits64* x);
  static void test_bits64_set(const bits64* x);
  static void test_bits64_index_bit(const bits64* x);
  static void test_bits64_counting(const bits64* x);
  static void test_bits64_vertical_line(const bits64* x);
  static void test_bits64_rotate(const bits64* x);
  static void test_bits64_is_subset_of_mask(const bits64* x,const bits64* y);
  static void test_bits64_to_ascii(const bits64* x);
  static void test_bits64_default_ctor();
  static void test_bits64_static_const_members();
  static void test_bits64_test_all();
  static void test_bits64_only_bit_index();
  
  static void test_board_copy_ctor(const board* x);
  static void test_board_ctor_string(const board* x);
  static void test_board_operator_assign(const board* x);
  static void test_board_operator_equals(const board* x,const board* y);
  static void test_board_operator_unequals(const board* x,const board* y);
  static void test_board_operator_less(const board* x,const board* y);
  static void test_board_switch_turn(const board* x);
  static void test_board_is_valid_move(const board* x);
  static void test_board_get_valid_moves(const board* x);
  static void test_board_do_random_moves(const board* x);
  static void test_board_reset();
  static void test_board_xot();
  static void test_board_get_children(const board* x);
  static void test_board_get_children_with_moves(const board* x);
  static void test_board_has_valid_moves(const board* x);
  static void test_board_count_valid_moves(const board* x);
  static void test_board_opponent_has_moves(const board* x);
  static void test_board_count_opponent_moves(const board* x);
  static void test_board_get_frontier_discs(const board* x);
  static void test_board_get_empty_fields(const board* x);
  static void test_board_get_non_empty_fields(const board* x);
  static void test_board_count_discs(const board* x);
  static void test_board_count_empty_fields(const board* x);
  static void test_board_to_ascii_art(const board* x);
  static void test_board_show(const board* x);
  static void test_board_get_disc_diff(const board* x);
  static void test_board_do_move(const board* x);
  static void test_board_position_to_index(const board* x);
  static void test_board_undo_move(const board* x);
  static void test_board_only_similar_siblings(const board* x);
  static void test_board_to_string(const board* x);
  static void test_board_to_database_string(const board* x);
  static void test_board_get_move_index(const board* x);
  static void test_board_to_database_board(const board* x);
  static void test_board_rotate(const board* x);
  static void test_board_get_rotation(const board* x);
  static void test_board_get_mobility(const board* x);
  static void test_board_hash(const board* x,const board* y);
  static void test_board_validity(const board* b);
  
  static void test_bits64_all();
  static void test_board_all();
  
 

  
  
  
public:
  
  // run all unittests
  void run();
};
