#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <gtkmm-3.0/gtkmm.h>

class PAEqualizer;

class MainWindow : public Gtk::Window
{
public:
  MainWindow ();
  virtual ~MainWindow ();
private:
  std::unique_ptr<PAEqualizer> equalizer;
};

#endif /* MAINWINDOW_H_ */
