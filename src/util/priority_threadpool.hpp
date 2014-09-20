#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <memory>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <tuple>
#include <iostream>

#include <unistd.h>

/*
 * Based on code by Paolo Brandoli
 * copied from http://codereview.stackexchange.com/questions/36018/thread-pool-on-c11
 */
class priority_threadpool
{
public: 
  
  typedef std::function<void()> job_t;
  
  struct job_data{
    job_data() = default;
    job_data(job_t f,job_t s,int p): first(f),second(s),priority(p){}
    
    bool operator<(const job_data& rhs) const{ return priority < rhs.priority; }
    
    job_t first,second;
    int priority;
  };
  


  // ctor: use n threads 
  priority_threadpool(size_t n);

  // dtor: returns after all running jobs have terminated
  // WARNING: the job queue may not be empty before returning
  virtual ~priority_threadpool();


  // add a job to the queue
  void add_job(job_t job,job_t notification_job,int prio);

  // returns whether all remaining jobs have STARTED
  bool empty() const;
  
  // returns amount of jobs in the queue
  int get_job_count();
  
  // lets workers actually start 
  // this may be used to fill the pqueue first completely
  void start_workers();
  
private:
  // each worker runs this method
  void loop();

  // returns a job
  // if the job queue is empty and terminate is not true it blocks
  bool get_next_job(job_data* out);

  // the workers
  std::vector<std::unique_ptr<std::thread> > workers;

  // job queue
  std::priority_queue<job_data> jobs;

  // mutex for job queue
  std::mutex lock_jobs_list;

  // condition variable used to notify that a new job has been
  // inserted in the job queue 
  std::condition_variable notify_job;

  // set to true by the class destructor to signal
  // the worker threads that they have to terminate.
  std::atomic<bool> terminate;

  std::atomic<bool> start_workers_flag;
  
};