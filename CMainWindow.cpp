#include "CMainWindow.hpp"

#define     EXPECTED_GUI_ITEMS   3
#define     DEFAULT_MODE    "Find closest note"


CMainWindow::CMainWindow(IUpdater &obj)
    : mNotify(obj)
    , mBox(Gtk::Orientation::VERTICAL, EXPECTED_GUI_ITEMS)
{
    // Main window init
    set_title("Audio Tuner");
    set_default_size(mWinSizeX, mWinSizeY);
    set_resizable(false);
    signal_show().connect( sigc::mem_fun(*this, &CMainWindow::on_window_show) );

    // Children init
    set_child(mBox);
    initButton();
    initLabel();
    initComboBox();
}

CMainWindow::~CMainWindow()
{
}

void CMainWindow::updateNotes(tNoteBuf &&notes)
{
    mNotes = notes;
    resetList();
}

void CMainWindow::on_window_show()
{
    mNotify.updateOnShow();
}

void CMainWindow::updateOutput(std::string text)
{
    mLabel.set_text(text);
}

////////// Button
void CMainWindow::initButton()
{
    mButton.signal_toggled().connect( sigc::mem_fun(*this, &CMainWindow::on_button_toggled) );
    mButton.set_margin(5);
    mButton.set_label(DEFAULT_MODE);
    mBox.append(mButton);
    mButton.set_expand();
}

void CMainWindow::on_button_toggled()
{
    mUserSelectedNote = mButton.get_active();
    mButton.set_label(mUserSelectedNote ? "Use selected note" : DEFAULT_MODE);
    mCombo.set_sensitive(mUserSelectedNote);
    mNotify.updateMode(mUserSelectedNote);
}

/////////// Label
void CMainWindow::initLabel()
{
    mLabel.set_text("Please, play ...");
    mLabel.set_margin(5);
    mBox.append(mLabel);
    mLabel.set_expand();
}

/////////// ComboBox
void CMainWindow::on_combo_changed()
{
    const auto iter = mCombo.get_active();
    if(iter)
    {
        const auto row = *iter;
        if(row)
        {
            mNotify.updateNote(row[mColumns.col_id]);
        }
    }
}

void CMainWindow::resetList()
{
    mRefTreeModel->clear();
    fillupList();
}

void CMainWindow::fillupList()
{
    auto iter = mRefTreeModel->append();
    auto row = *iter;
    
    int cnt = 0;
    for (auto i : mNotes)
    {
        if (cnt > 0)    row = *(mRefTreeModel->append());
        row[mColumns.col_id] = cnt;
        row[mColumns.col_name] = i.name + " (" + std::to_string(i.freq) + "Hz)";
        if (0 == cnt)   mCombo.set_active(iter);
        ++cnt; 
    }
}

void CMainWindow::initComboBox()
{
    mRefTreeModel = Gtk::ListStore::create(mColumns);
    mCombo.set_model(mRefTreeModel);
  
    //Add the model column to view it
    mCombo.pack_start(mColumns.col_name);

    //Connect signal handler
    mCombo.signal_changed().connect( sigc::mem_fun(*this, &CMainWindow::on_combo_changed) );

    mBox.append(mCombo);
    mCombo.set_expand();
    mCombo.set_sensitive(mUserSelectedNote);
}

