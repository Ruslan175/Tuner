#ifndef  _C_CONFIG_HPP
#define  _C_CONFIG_HPP

#include <string>
#include "json.hpp"
#include "types.h"


class CConfig
{
    public:
    CConfig();
    bool loadFromFile(std::string jname);
    double getMaxSoundFreq() const;
    tNoteBuf getNoteData() const;

    private:
    double              mMaxSoundFreq{0.0};
    const double        mDefaultMaxSoundFreq{18000.0};
    tNoteBuf            mNotes;
    tNoteBuf            mDefaultNotes;   

    bool getFileJson(std::string jname, nlohmann::json &doc);
};

#endif
