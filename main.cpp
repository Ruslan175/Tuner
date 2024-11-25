#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include "CMicrophone.hpp"
#include "CSound.hpp"
#include "CNote.hpp"
#include "CMainWindow.hpp"
#include "CPostman.hpp"


#define     JSON_CONFIG_FILE        "../config.json"
#define     _SLEEP_MS(_x)           std::this_thread::sleep_for(std::chrono::milliseconds(_x));

using namespace std;

void init();
void runSoundRead();
void runUserInterface();

mutex   gMutex;
condition_variable gCondVar;
CConfig gAppConfig;
tGtkApp guiApp{nullptr};    // the same as shared_ptr<>
shared_ptr<CPostman> refPostman{nullptr};


int main()
{
    init();
    if (true == CMicrophone::isReady())
    {
        thread t1(runUserInterface);    // GUI thread
        thread t2(runSoundRead);        // Backend thread

        t1.join();
        t2.join();
    }
    else cout << "Error: App cannot be run as microphone is not ready" << endl;

    return 0;
}


void init()
{
    bool r = gAppConfig.loadFromFile( JSON_CONFIG_FILE );    
    cout << "JSON config data is " << (r ? "OK" : "NOK") << endl;
    if (false == r) cout << "Warning: The default config data will be used" << endl;
}


void runUserInterface() 
{
    do
    {
        guiApp = Gtk::Application::create("gtkmm.tuner");
        if (nullptr == guiApp)
        {
            cout << "Error: App cannot be run due to GUI fault" << endl;
            break;
        }
        
        unique_lock  ul(gMutex);
        refPostman.reset(new CPostman(guiApp, gAppConfig, ul, gCondVar));
        if (nullptr == refPostman)
        {
            cout << "Error: App cannot be run due to new-oparator fault" << endl;
            break;
        }

        guiApp->make_window_and_run<CMainWindow>(0, NULL, *refPostman);
    }
    while(false);
}


void runSoundRead() 
{
    // Wait when GUI is ready -- use CV
    {
        unique_lock ul(gMutex);
        gCondVar.wait_for(ul, std::chrono::seconds(10), []{ return (nullptr != refPostman) && (true == refPostman->isReady()); });
    }

    if (nullptr != refPostman)
    {
        // Create & configure objects
        tSoundData bf;
        CSound snd(CMicrophone::getDataRate());
        CNote note(*refPostman);
        snd.setUpFrequencyLimit( gAppConfig.getMaxSoundFreq() );
        note.setNotes( gAppConfig.getNoteData() );

        // Run main cycle until the GUI window is closed
        while(nullptr != guiApp->get_run_window())
        {
            CMicrophone::getSound(bf);
            note.handleSound( snd.getTopFrequency(bf) );
            _SLEEP_MS(300);
        }
    }
}

