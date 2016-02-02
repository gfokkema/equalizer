#include "mainwindow.h"
#include "pa-equalizer.h"

int main(int argc, char* argv[])
{
  auto equalizer = std::shared_ptr<PAEqualizer> (new PAEqualizer ());
  auto sink = equalizer->get_sinks ().front ();
  equalizer->connect_to_sink (sink);

  auto app = Gtk::Application::create(argc, argv, "org.equalizer");
  MainWindow window (equalizer);
  return app->run(window);
}
