#ifndef PA_EQUALIZER_H_
#define PA_EQUALIZER_H_

#include <giomm.h>
#include <glibmm.h>
#include <memory>
#include <vector>
#include "pa-channel.h"

typedef Glib::Variant<Glib::ustring> var_ustring;
typedef Glib::Variant<std::vector<Glib::ustring>> var_vector_ustring;

class PAEqualizer
{
public:
  PAEqualizer ();
  virtual ~PAEqualizer ();
  void on_pulseaudio_dbus (Glib::RefPtr<Gio::AsyncResult>& result);
private:
  Glib::RefPtr<Gio::DBus::Connection> conn;
  Glib::RefPtr<Gio::DBus::Proxy> proxy;
  std::vector<PAChannel> channels;
};

class PAException : public std::exception
{
public:
  PAException (std::string error)
  : m_error (error) {};

  ~PAException () throw () {};

  const char* what () const throw ()
  {
    return this->m_error.c_str();
  };
private:
  std::string m_error;
};
#endif /* PA_EQUALIZER_H_ */
