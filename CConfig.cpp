#include <fstream>
#include <iostream>
#include "CConfig.hpp"

// JSON info
// https://github.com/nlohmann/json/issues/1703
// sudo apt-get install nlohmann-json3-dev
// https://www.studyplan.dev/pro-cpp/json
// https://json.nlohmann.me/api/basic_json/dump/

#define     JSON_MAX_SOUND_FREQ    "MaxSoundFreq"
#define     JSON_NOTE_DATA         "Notes"

using json = nlohmann::json;


CConfig::CConfig()
{
    mDefaultNotes.push_back(CNoteData("Do",  262));
    mDefaultNotes.push_back(CNoteData("Re",  294));
    mDefaultNotes.push_back(CNoteData("Mi",  330));
    mDefaultNotes.push_back(CNoteData("Fa",  349));
    mDefaultNotes.push_back(CNoteData("Sol", 392));
    mDefaultNotes.push_back(CNoteData("La",  440));
    mDefaultNotes.push_back(CNoteData("Si",  494));
    mDefaultNotes.shrink_to_fit();
}


bool CConfig::getFileJson(std::string jname, json &doc)
{
    bool ret = false;
    std::ifstream fl;
    fl.open(jname);
    if (true == fl.is_open()) 
    {
        try
        {
            json tmp{json::parse(fl)};
            doc = tmp;
            fl.close();
            ret = true;
        }
        catch(...)
        {
            fl.close();
            std::cout << "An exception has been generated on json-file reading." << std::endl
                 << "The default config will be used" << std::endl;
        }
    }
    else std::cout << "JSON file cannot be opened!" << std::endl;

    return ret;
}


bool CConfig::loadFromFile(std::string jname)
{
    bool ready = false;
    do
    {
        json doc;
        if (false == getFileJson(jname, doc))  break;

        // Load MaxSoundFreq
        mMaxSoundFreq = doc[0].value( JSON_MAX_SOUND_FREQ, -1.0);
        if (0.0 > mMaxSoundFreq)
        {
            std::cout << "JSON: Key " << JSON_MAX_SOUND_FREQ << " was not found !" << std::endl;
            break;
        }

        // Load notes data
        auto it = doc[0].find( JSON_NOTE_DATA );
        if (doc[0].end() != it)
        {
            try
            {
                for (auto i : *it)
                {
                    std::string s = i["name"].get<std::string>();
                    int freq = i["frequency"].get<int>();
                    mNotes.push_back(CNoteData(s, freq));
                }
            }
            catch(...)
            {
                std::cout << "JSON: Notes data is corrupted!" << std::endl;
                break;
            }
        }
        else 
        {
            std::cout << "JSON: Key " << JSON_NOTE_DATA << " was not found!" << std::endl;
            break;
        }

        // If json data is OK
        ready = true;
    }
    while(false);

    if (true == ready)  mNotes.shrink_to_fit();
    else
    {
        mNotes = mDefaultNotes;
        mMaxSoundFreq = mDefaultMaxSoundFreq;
    }
  
    return ready;
}


double CConfig::getMaxSoundFreq() const
{
    return mMaxSoundFreq;
}


tNoteBuf CConfig::getNoteData() const
{
    return mNotes;
}

