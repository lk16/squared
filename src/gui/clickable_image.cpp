#include "gui/clickable_image.hpp"
#include "gui/main_window.hpp"

clickable_image::clickable_image():
  Gtk::EventBox()
{
}


void clickable_image::init(main_window* par, int _field_id, const std::string& imagefile) 
{
  add(image);
  show_all_children();
  set_image(imagefile);
  field_id = _field_id;
  parent = par;
}

bool clickable_image::on_button_press_event(GdkEventButton* e)
{
  (void) e;
  parent->control->on_human_do_move(field_id);
  
  return true;
}

void clickable_image::set_image(const std::string& filename)
{
  image.set(filename);
}
