#include "settings_dialog.hpp"

settings_dialog::settings_dialog(Gtk::Window& parent,int black_level,int white_level):
  Gtk::Dialog("Settings",parent),
  radio_w_bot("Robot"),
  radio_w_human("Human"),
  radio_b_bot("Robot"),
  radio_b_human("Human"),
  scale_w(1.0,11.0,1.0),
  scale_b(1.0,11.0,1.0),
  frame_w("White"),
  frame_b("Black")
{
  radio_b_bot.set_group(radio_group_b);
  radio_b_human.set_group(radio_group_b);
  radio_w_bot.set_group(radio_group_w);
  radio_w_human.set_group(radio_group_w);
  
  if(black_level==-1){
    radio_b_human.set_active();
  }
  else{
    radio_b_bot.set_active();
    scale_b.set_value((double)black_level);
  }
  
  if(white_level==-1){
    radio_w_human.set_active();
  }
  else{
    radio_w_bot.set_active();
    scale_w.set_value((double)white_level);
  }
  
  
  this->get_vbox()->add(frame_b);
  frame_b.add(vbox_b);
  vbox_b.add(radio_b_bot);
  vbox_b.add(radio_b_human);
  vbox_b.add(scale_b);
  
  this->get_vbox()->add(frame_w);
  frame_w.add(vbox_w);
  vbox_w.add(radio_w_bot);
  vbox_w.add(radio_w_human);
  vbox_w.add(scale_w);
  
  this->add_button("Ok",Gtk::RESPONSE_OK);
  
  this->set_default_size(200,200);
  
  this->show_all_children();

}

void settings_dialog::collect_data(int* black_level,int* white_level)
{
  if(radio_b_human.get_active()){
    *black_level = -1;
  }
  else{
    *black_level = (int)(scale_b.get_value());
  }
  
  if(radio_w_human.get_active()){
    *white_level = -1;
  }
  else{
    *white_level = (int)(scale_w.get_value());
  }
}
