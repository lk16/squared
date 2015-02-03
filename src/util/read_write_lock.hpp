#pragma once

#include <atomic>
#include <condition_variable>

/*
 * This code was based on code by toliuweijing: 
 * https://github.com/toliuweijing/Readers-Writer-Lock
 */

class read_write_lock{

public:
  read_write_lock();
  ~read_write_lock();
  void lock_read();
  void unlock_read();
  void lock_write();
  void unlock_write();

private:
  int num_readers;
  std::mutex mutex_num_readers;
  std::mutex mutex_writer;
  std::condition_variable_any cv_num_readers;
};

class rw_lock_read_guard{
  read_write_lock* lock;

public:
  rw_lock_read_guard(read_write_lock& rw);
  ~rw_lock_read_guard();
};


class rw_lock_write_guard{
  read_write_lock* lock;

public:
  rw_lock_write_guard(read_write_lock& rw);
  ~rw_lock_write_guard();
};