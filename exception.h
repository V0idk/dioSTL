#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include "mystring.h"


namespace mmm{
  class exception {
  public:
    exception(){}
    virtual ~exception(){}
    virtual const char* what() const {return "mmm::exception";}
  };


  class logic_error : public exception
  {
    string _M_msg;

  public:
    explicit logic_error( const string & __arg ) : _M_msg( __arg ){}


    explicit logic_error( const char* __arg): _M_msg( __arg ){}

    virtual ~logic_error(){}
    virtual const char* what() const
    {
      return(_M_msg.c_str());
    }
  };


  class out_of_range : public logic_error
  {
  public:
    explicit out_of_range( const string & __arg ) : logic_error( __arg ){}
    explicit out_of_range( const char* __arg): logic_error( __arg ){}
    virtual ~out_of_range(){}
  };

}


#endif