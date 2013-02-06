#include "field.hpp"
#include "mainwindow.hpp"


field::field(main_window* par, int _x, int _y, const std::string& imagefile): 
  Gtk::EventBox(),
  image(imagefile),
  x(_x),
  y(_y),
  parent(par)
{
  add(image);
  show_all_children();
}

bool field::on_button_press_event(GdkEventButton* _event)
{
  parent->control.on_human_do_move(x,y);
  _event = NULL; /* prevent compiler complaining */  
  return true;
}
