#pragma once

#include <cstdlib>

#define LOCATION __FILE__ << ':' << __LINE__ << '\t'

#ifndef NDEBUG

#define SHOW_VAR(a) std::cout << LOCATION << #a << " = " << (a) << std::endl 
#define SHOW_VAR2(a,b)     SHOW_VAR(a);      SHOW_VAR(b)
#define SHOW_VAR3(a,b,c)   SHOW_VAR2(a,b);   SHOW_VAR(c)
#define SHOW_VAR4(a,b,c,d) SHOW_VAR3(a,b,c); SHOW_VAR(d)
#define PING std::cout << LOCATION << "PING!\n"
#define CONTAINS_BUG std::cout << LOCATION << "Do not use this function, it contains bugs!\n"
#define NOT_IMPLEMENTED std::cout << LOCATION << "This is not implemented yet!\n" 
#define CRASH std::cout << LOCATION << "I'm crashing!\n"; abort()

#else

#define SHOW_VAR(a)
#define SHOW_VAR2(a,b)
#define SHOW_VAR3(a,b,c)
#define SHOW_VAR4(a,b,c,d)
#define PING
#define CONTAINS_BUG
#define NOT_IMPLEMENTED
#define CRASH
  
#endif // NDEBUG
