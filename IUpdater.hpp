#ifndef I_UPDATER_HPP
#define I_UPDATER_HPP


class IUpdater
{// from GUI
  public:
    virtual void updateMode(bool mode) = 0;
    virtual void updateNote(int idx) = 0;
    virtual void updateOnShow() = 0;
    virtual ~IUpdater() {};
};

#endif
