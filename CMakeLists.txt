cmake_minimum_required(VERSION 2.8)

project(squared)

find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-2.4)
pkg_check_modules(GLIBMM glibmm-2.4)
pkg_check_modules(GTK_PLUS gtk+-2.0)
pkg_check_modules(GDK_PIXBUF gdk-pixbuf-2.0)
pkg_check_modules(GIOMM giomm-2.4)
pkg_check_modules(GDKMM gdkmm-2.4)


set(WARNING_FLAGS
  "-pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused"
)

set(COMMON_FLAGS
  "-I./src -std=c++0x -pthread ${WARNING_FLAGS}"
)


set(OPTIMISE_FLAGS
    "-pipe -msse2 -fforce-addr -ftree-vectorize -funroll-loops"
)


set(DEBUG_FLAGS
  "${COMMON_FLAGS} -g"
)

set(OPTIMISED_DEBUG_FLAGS
  "${COMMON_FLAGS} ${OPTIMISE_FLAGS} -g -O2" 
)

set(OPTIMISING_FLAGS
  "${COMMON_FLAGS} ${OPTIMISE_FLAGS} -DNDEBUG -O3" 
)

set(CMAKE_CXX_FLAGS 
  ${OPTIMISING_FLAGS}
  #${OPTIMISED_DEBUG_FLAGS}
  #${DEBUG_FLAGS}
) 

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${GTKMM_LIBRARY_DIRS})
include_directories(SYSTEM ${GTKMM_INCLUDE_DIRS})

include_directories(${GLIBMM_LIBRARY_DIRS})
include_directories(SYSTEM ${GLIBMM_INCLUDE_DIRS})

include_directories(${GDK_PIXBUF_LIBRARY_DIRS})
include_directories(SYSTEM ${GDK_PIXBUF_INCLUDE_DIRS})

include_directories(${GTK_PLUS_LIBRARY_DIRS})
include_directories(SYSTEM ${GTK_PLUS_INCLUDE_DIRS})

include_directories(${GIOMM_LIBRARY_DIRS})
include_directories(SYSTEM ${GIOMM_INCLUDE_DIRS})

include_directories(${GDKMM_LIBRARY_DIRS})
include_directories(SYSTEM ${GDKMM_INCLUDE_DIRS})


file(GLOB SOURCE_FILES
    "src/*.cpp"
    "src/*/*.cpp"
)

add_executable(squared
  ${SOURCE_FILES}
)

target_link_libraries(squared
  ${GTKMM_LIBRARIES}
  ${GLIBMM_LIBRARIES}
  ${GDK_PIXBUF_LIBRARIES}
)
