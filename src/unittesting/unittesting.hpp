#pragma once

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "game/board.hpp"
#include "util/math.hpp"

#define add_to_fun_vec(vec,f) \
    vec.push_back(std::make_pair(squared_unittest::f,#f))


class squared_unittest{
  void announce(const std::string& name);
  void announce_step();
  
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

  void check_board(const board* b);
  
public:
  
  // run all unittests
  void run();
};
