project(squared)

find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-3.0)
pkg_check_modules(GLIBMM glibmm-2.4)
pkg_check_modules(GTK_PLUS gtk+-2.0)
pkg_check_modules(GDK_PIXBUF gdk-pixbuf-2.0)


set(DEBUG_FLAGS
  "-I .. -std=c++0x -Wall -Wextra -g -Wfloat-equal -Wno-unused-result -Wshadow -Wstrict-aliasing"
)

set(OPTIMIZED_DEBUG_FLAGS
  "-I .. -std=c++0x -Wall -DNDEBUG -Wextra -O2 -g -pipe -msse2 -fforce-addr -ftree-vectorize -funroll-loops" 
)

set(OPTIMIZING_FLAGS
  "-I .. -std=c++0x -Wall -DNDEBUG -Wextra -O3 -pipe -msse2 -fforce-addr -ftree-vectorize -funroll-loops" 
)

set(CMAKE_CXX_FLAGS 
  "${OPTIMIZING_FLAGS}"
  #"${OPTIMIZED_DEBUG_FLAGS}"
  #"${DEBUG_FLAGS}"
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

add_executable(../squared
  bots/bot_ali.cpp
  bots/bot_ali_trainer.cpp
  bots/bot_random.cpp
  bots/bot_register.cpp
  book/book.cpp
  game/board.cpp
  game/board_move.cpp
  game/game_control.cpp
  gui/main_window.cpp
  gui/preferences_dialog.cpp
  gui/clickable_image.cpp
  main/args.cpp
  main/squared.cpp
  main/testing.cpp
  util/bitset.cpp
  util/csv.cpp
  util/string.cpp
)

target_link_libraries(../squared
  ${GTKMM_LIBRARIES}
  ${GLIBMM_LIBRARIES}
  ${GDK_PIXBUF_LIBRARIES}
)
