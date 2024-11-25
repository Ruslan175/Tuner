#include <string>
#include "CNote.hpp"


CNote::CNote(ISender& s)
    : mNotes()
    , mSender(s)
{
}

void CNote::setNotes(tNoteBuf &&notes)
{
    mNotes = notes;
}


int CNote::getNearestNote(int f)
{
    int df = std::abs(f - mNotes[0].freq);
    int idx = 0;
    for (unsigned int i = 1; i < mNotes.size(); ++i)
    {
        int d = std::abs(f - mNotes[i].freq);
        if (d < df)
        {
            df = d;
            idx = i;
        }
    }
    return idx;
}


void CNote::handleSound(int f)
{
    std::string out;
    if (true == mSender.getMode()) // A note has been selected by user
    {
        int idx = mSender.getNote();
        int diff = mNotes[idx].freq - f;
        out = "delta " + std::to_string(diff) + " Hz";
    }
    else // Just find the closest note
    {
        int idx = getNearestNote(f);
        int diff = mNotes[idx].freq - f;
        out = mNotes[idx].name + ", " + std::to_string(mNotes[idx].freq) + "Hz: delta " + std::to_string(diff) + " Hz";
    }
    mSender.sendOutput(out);
}

