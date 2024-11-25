#ifndef I_SENDER_HPP
#define I_SENDER_HPP

#include <string>

class ISender
{ // to GUI
  public:
    virtual void sendOutput(std::string &text) = 0;
    virtual bool getMode() const = 0;
    virtual int  getNote() const = 0;
    virtual bool isReady() const = 0;
    virtual ~ISender() {};
};

#endif
