find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-2.4) # look into FindPkgConfig.cmake, 



link_directories(
  ${GTKMM_LIBRARY_DIRS}
)

#set(CMAKE_CXX_FLAGS "-Wall -Wextra -g -O0") 
 set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3 -DNDEBUG") 

SET(objects
  board.cpp
  bot_base.cpp
  bot_random.cpp
  bot_ali.cpp
  field.cpp
  gamecontrol.cpp
  main.cpp
  mainwindow.cpp
)


include_directories(.)
include_directories(${GTKMM_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})


ADD_EXECUTABLE(../squared
  ${objects}
)
target_link_libraries(../squared
  ${GTKMM_LIBRARIES}
)