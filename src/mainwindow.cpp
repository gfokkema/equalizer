#include "mainwindow.h"

#include "channelslider.h"
#include "pa-equalizer.h"

MainWindow::MainWindow()
{
  this->equalizer = std::unique_ptr<PAEqualizer> (new PAEqualizer ());

  this->set_border_width(10);
  this->set_default_size(400, 200);

  auto *hbox = Gtk::manage (new Gtk::Box ());
  hbox->set_spacing (20);

  for (int i = 0; i < 10; i++)
  {
    auto *slider = Gtk::manage (new ChannelSlider ());
    hbox->add (*slider);
  }

  this->add (*hbox);
  this->show_all ();
}

MainWindow::~MainWindow()
{
}
