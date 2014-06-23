#pragma once

#include <utility>

template<class K,class V,int size>
class hash_table{

public:

  typedef unsigned long long(*hasher_t)(const K);
  
  hasher_t hasher;
  
  hash_table(hasher_t h){
    data = new std::pair<K,V>[size];
    hasher = h;
  }
  
  ~hash_table(){
    delete[] data;
  }
  
  void add(const K k,const V v){
    unsigned long long i = hasher(k) % size;
    data[i] = std::pair<K,V>(k,v);
  }
  
  V* lookup(const K& k){
    unsigned long long i = hasher(k) % size;
    if(data[i].first == k){
      return &data[i].second; 
    }
    return NULL;
  }
  
  
  
private:
  std::pair<K,V>* data;
  
  
  
};