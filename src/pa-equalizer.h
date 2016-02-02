#ifndef PA_EQUALIZER_H_
#define PA_EQUALIZER_H_

#include <giomm.h>
#include <glibmm.h>
#include <memory>
#include <vector>
#include "pa-channel.h"

#define DBUS_EQUALIZER_IFACE "org.PulseAudio.Ext.Equalizing1.Equalizer"
#define DBUS_LOOKUP_IFACE "org.PulseAudio.ServerLookup1"
#define DBUS_MANAGER_IFACE "org.PulseAudio.Ext.Equalizing1.Manager"
#define DBUS_PROPERTIES_IFACE "org.freedesktop.DBus.Properties"

#define DBUS_EQUALIZER_OBJPATH "/org/pulseaudio/equalizing1"
#define DBUS_LOOKUP_OBJPATH "/org/pulseaudio/server_lookup1"

typedef Glib::Variant<Glib::Variant<std::vector<Glib::ustring>>> var_var_vec_ustring;
typedef Glib::Variant<Glib::Variant<Glib::ustring>> var_var_ustring;
typedef Glib::Variant<Glib::ustring> var_ustring;
typedef Glib::Variant<std::vector<Glib::ustring>> var_vec_ustring;

class PAEqualizer
{
public:
  PAEqualizer ();
  virtual ~PAEqualizer ();

  void                       connect_to_sink (Glib::ustring);
  std::vector<Glib::ustring> get_sinks ();
  void                       print_sinks ();
private:
  Glib::RefPtr<Gio::DBus::Connection> conn;
  Glib::RefPtr<Gio::DBus::Proxy>      proxy;
  std::vector<PAChannel>              channels;
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
