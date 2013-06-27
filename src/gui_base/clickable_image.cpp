#include "gui_base/clickable_image.hpp"

clickable_image::clickable_image(main_window* par, int _field_id, const std::string& imagefile): 
  Gtk::EventBox(),
  image(imagefile),
  field_id(_field_id),
  parent(par)
{
  add(image);
  show_all_children();
}

bool clickable_image::on_button_press_event(GdkEventButton* _event)
{
  parent->control.on_human_do_move(field_id);
  _event = NULL; /* prevent compiler complaining */  
  return true;
}

void clickable_image::set(const std::string& filename)
{
  image.set(filename);
}
