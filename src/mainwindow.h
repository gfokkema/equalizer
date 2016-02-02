#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <gtkmm-3.0/gtkmm.h>

class PAEqualizer;

class MainWindow : public Gtk::Window
{
public:
  MainWindow (std::shared_ptr<PAEqualizer>);
  virtual ~MainWindow ();
private:
  std::shared_ptr<PAEqualizer> m_equalizer;
};

#endif /* MAINWINDOW_H_ */
