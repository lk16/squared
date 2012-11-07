find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-2.4) # look into FindPkgConfig.cmake, 



link_directories(
  ${GTKMM_LIBRARY_DIRS}
)

  set(DEBUG_FLAGS
    "-g -O2 -Wall -Wextra -Wfloat-equal -Wno-unused-result -Wshadow -Wstrict-aliasing "
  )

  set(OPTIMIZING_FLAGS
    "-O3 -DNDEBUG -pipe -msse2 -fforce-addr -fprefetch-loop-arrays -funroll-loops" 
  )


set(CMAKE_CXX_FLAGS 
# ${OPTIMIZING_FLAGS}
  ${DEBUG_FLAGS} 
) 

SET(objects
  board.cpp
  bot_base.cpp
  bot_ali.cpp
  field.cpp
  gamecontrol.cpp
  mainwindow.cpp
)


include_directories(.)
include_directories(${GTKMM_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})


ADD_EXECUTABLE(../squared
  ${objects}
  main.cpp
)

target_link_libraries(../squared
  ${GTKMM_LIBRARIES}
  pthread
)