#ifndef PA_EXCEPTION_H_
#define PA_EXCEPTION_H_

#include <memory>

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

#endif /* PA_EXCEPTION_H_ */
