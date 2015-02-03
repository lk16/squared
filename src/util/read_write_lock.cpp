#include "util/read_write_lock.hpp"


read_write_lock::read_write_lock(){
  num_readers = 0;
}

read_write_lock::~read_write_lock(){
}

void read_write_lock::lock_read(){
  std::lock(mutex_writer, mutex_num_readers);
  num_readers++;
  mutex_writer.unlock();
  mutex_num_readers.unlock();
}

void read_write_lock::unlock_read(){
  std::unique_lock<std::mutex> lock(mutex_num_readers);
  num_readers--;
  if (num_readers == 0){
    cv_num_readers.notify_one();
  }
}

void read_write_lock::lock_write() {
  std::lock(mutex_writer, mutex_num_readers);
  cv_num_readers.wait(mutex_num_readers, [&]{ return num_readers == 0; });
}

void read_write_lock::unlock_write() {
  cv_num_readers.notify_one();
  mutex_writer.unlock();
  mutex_num_readers.unlock();
}

rw_lock_read_guard::rw_lock_read_guard(read_write_lock& rw)
{
  lock = &rw;
  lock->lock_read();
}

rw_lock_read_guard::~rw_lock_read_guard()
{
  lock->unlock_read();
}

rw_lock_write_guard::rw_lock_write_guard(read_write_lock& rw)
{
  lock = &rw;
  lock->lock_write();
}

rw_lock_write_guard::~rw_lock_write_guard()
{
  lock->unlock_write();
}



