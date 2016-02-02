#ifndef PA_EQUALIZER_H_
#define PA_EQUALIZER_H_

#include <giomm.h>
#include <glibmm.h>
#include <iostream>
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
private:
  Glib::RefPtr<Gio::DBus::Connection> conn;
  Glib::RefPtr<Gio::DBus::Proxy> proxy;
  std::vector<PAChannel> channels;
};

class PAProxy : public Gio::DBus::Proxy
{
  void on_signal (const Glib::ustring& sender_name,
                  const Glib::ustring& signal_name,
                  const Glib::VariantContainerBase& parameters)
  {
    std::cout << signal_name << std::endl;
  };
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
