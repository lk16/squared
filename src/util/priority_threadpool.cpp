#include "priority_threadpool.hpp"

priority_threadpool::priority_threadpool(size_t n):
  workers(n),
  terminate(false),
  start_workers_flag(false)
{
  for(auto& worker: workers){
    worker.reset(new std::thread(&priority_threadpool::loop, this));
  }
}

priority_threadpool::~priority_threadpool()
{
  std::unique_lock<std::mutex> lock_list(lock_jobs_list);
  terminate = true;
  notify_job.notify_all();
  lock_list.unlock();
  
  for(auto& worker: workers){
    worker->join();
  }
}

int priority_threadpool::get_job_count()
{
  std::unique_lock<std::mutex> lock_list(lock_jobs_list);
  return jobs.size();
}

void priority_threadpool::start_workers()
{
  start_workers_flag = true;
}



void priority_threadpool::add_job(job_t job,int prio)
{
  std::unique_lock<std::mutex> lock_list(lock_jobs_list);
  jobs.push(priority_threadpool::job_data(job,prio));
  notify_job.notify_one();
}

bool priority_threadpool::get_next_job(job_data* out)
{
  while(!start_workers_flag){
    sleep(1);
  }
  
  std::unique_lock<std::mutex> lock_list(lock_jobs_list);
  
  while(!terminate){
    
    if(!jobs.empty()){
      job_data job = jobs.top();
      jobs.pop();
      *out = job;
      return true;
    }
    notify_job.wait(lock_list);
  }
  return false;
}

void priority_threadpool::loop()
{
  job_data job;
  while(get_next_job(&job)){
    job.first();
  }
}

bool priority_threadpool::empty() const
{
  return jobs.empty();
}