#include "CPostman.hpp"


CPostman::CPostman(tGtkApp &app, CConfig &cnf, std::unique_lock<std::mutex> &ul, std::condition_variable &cv)
    : mApp(app)
    , mConfig(cnf)
    , mUniqLock(ul)
    , mCondVar(cv)
{
}

CPostman::~CPostman()
{
}

void CPostman::updateMode(bool mode)
{
    mMode.store(mode);
}


void CPostman::updateNote(int idx)
{
    mNote.store(idx);
}


CMainWindow* CPostman::getWindow()
{
    return dynamic_cast<CMainWindow*>(mApp->get_run_window());
}


void CPostman::updateOnShow()
{ 
    CMainWindow *ptr = getWindow();	
    if (nullptr != ptr) 
    {// Apply config data
        ptr->updateNotes( mConfig.getNoteData() );
    }

    // Unblock backend thread
    mReady.store(true);
    mUniqLock.unlock();
    mCondVar.notify_one();
}


void CPostman::sendOutput(std::string &text)
{
    CMainWindow *ptr = getWindow();	
    if (nullptr != ptr) 
    {
        ptr->updateOutput(text);
    }
}


bool CPostman::getMode() const
{
    return mMode.load();
}


int CPostman::getNote() const
{
    return mNote.load();
}


bool CPostman::isReady() const
{
    return mReady.load();
}
