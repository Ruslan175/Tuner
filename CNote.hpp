#ifndef  _C_NOTE_HPP
#define  _C_NOTE_HPP

#include "ISender.hpp"
#include "types.h"


class CNote
{
    public:
    CNote(ISender& s);
    void handleSound(int f);
    void setNotes(tNoteBuf &&notes);

    private:
    tNoteBuf    mNotes;
    ISender&    mSender;

    int getNearestNote(int f);
};

#endif
