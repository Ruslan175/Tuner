#ifndef CPOSTMAN_HPP
#define CPOSTMAN_HPP

#include <gtkmm/application.h>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include "CMainWindow.hpp"
#include "IUpdater.hpp"
#include "ISender.hpp"
#include "CConfig.hpp"


typedef Glib::RefPtr<Gtk::Application>  tGtkApp;


class CPostman : public IUpdater, public ISender
{// from GUI
    public:
    CPostman(tGtkApp &app, CConfig &cnf, std::unique_lock<std::mutex> &ul, std::condition_variable &cv);
    virtual ~CPostman();
    
    // IUpdater
    virtual void updateMode(bool mode) override;
    virtual void updateNote(int idx) override;
    virtual void updateOnShow() override;
    // ISender
    virtual void sendOutput(std::string &text) override;
    virtual bool getMode() const override;
    virtual int  getNote() const override;
    virtual bool isReady() const override;

    private:
    tGtkApp&                 mApp;
    CConfig&                 mConfig;
    std::atomic<bool>        mMode{false};
    std::atomic<int>         mNote{0};
    std::atomic<bool>        mReady{false};
    std::unique_lock<std::mutex>&   mUniqLock;
    std::condition_variable&        mCondVar;

    CMainWindow* getWindow();
};

#endif
