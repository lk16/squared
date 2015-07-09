cmake_minimum_required(VERSION 2.8)

project(squared)

find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-2.4)
pkg_check_modules(GLIBMM glibmm-2.4)
pkg_check_modules(GTK_PLUS gtk+-2.0)
pkg_check_modules(GDK_PIXBUF gdk-pixbuf-2.0)
pkg_check_modules(GIOMM giomm-2.4)
pkg_check_modules(GDKMM gdkmm-2.4)

set(DEBUG_FLAGS
  "-I .. -std=c++0x -pthread -Wall -Wextra -g -Wfloat-equal -Wno-unused-result -Wshadow -Wstrict-aliasing"
)

set(OPTIMIZED_DEBUG_FLAGS
  "-I .. -std=c++0x -pthread -Wall -Wextra -g -O2 -pipe -msse2 -fforce-addr -ftree-vectorize -funroll-loops" 
)

set(OPTIMIZING_FLAGS
  "-I .. -std=c++0x -pthread -Wall -Wextra -DNDEBUG -O3 -pipe -msse2 -fforce-addr -ftree-vectorize -funroll-loops" 
)

set(CMAKE_CXX_FLAGS 
  ${OPTIMIZING_FLAGS}
  #${OPTIMIZED_DEBUG_FLAGS}
  #${DEBUG_FLAGS}
) 

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

include_directories(${GLIBMM_LIBRARY_DIRS})
include_directories(${GLIBMM_INCLUDE_DIRS})

include_directories(${GDK_PIXBUF_LIBRARY_DIRS})
include_directories(${GDK_PIXBUF_INCLUDE_DIRS})

include_directories(${GTK_PLUS_LIBRARY_DIRS})
include_directories(${GTK_PLUS_INCLUDE_DIRS})

include_directories(${GIOMM_LIBRARY_DIRS})
include_directories(${GIOMM_INCLUDE_DIRS})

include_directories(${GDKMM_LIBRARY_DIRS})
include_directories(${GDKMM_INCLUDE_DIRS})

  



file(GLOB SOURCE_FILES
    "*.cpp"
    "*/*.cpp"
)

add_executable(../squared
  ${SOURCE_FILES}
)

target_link_libraries(../squared
  ${GTKMM_LIBRARIES}
  ${GLIBMM_LIBRARIES}
  ${GDK_PIXBUF_LIBRARIES}
)
