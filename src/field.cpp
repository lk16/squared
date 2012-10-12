#include "field.hpp"
#include "mainwindow.hpp"


field::field(main_window* par, int x, int y, const std::string& imagefile): 
  Gtk::EventBox(),
  image(imagefile),
  x(x),
  y(y),
  parent(par)
{
  add(image);
  show_all_children();
}

void field::update_image(const char* imagefile)
{
  image.set(imagefile);
}


bool field::on_button_press_event(GdkEventButton* event)
{
  parent->control.on_human_do_move(x,y);
  event = NULL; /* prevent compiler complaining */  
  return true;
}
