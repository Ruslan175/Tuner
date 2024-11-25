#ifndef GTKMM_CMAINWINDOW_HPP
#define GTKMM_CMAINWINDOW_HPP

#include <gtkmm.h>
#include <gtkmm/window.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/liststore.h>
#include <gtkmm/box.h>
#include "types.h"
#include "IUpdater.hpp"


class CMainWindow : public Gtk::Window
{
    public:
    CMainWindow(IUpdater &obj);
    void updateNotes(tNoteBuf &&notes);
    void updateOutput(std::string text);
    virtual ~CMainWindow();

    private:
    /////// Main window related items
    IUpdater& mNotify;
    int mWinSizeX{220};
    int mWinSizeY{150};
    void on_window_show();
    CMainWindow() = delete;

    /////// User actions data
    bool mUserSelectedNote{false};
    tNoteBuf  mNotes;

    /////// Box related items
    Gtk::Box mBox;

    /////// Button/checkbox related items
    Gtk::CheckButton mButton;
    void initButton();
    void on_button_toggled();

    /////// Label related items
    Gtk::Label mLabel;
    void initLabel();

    /////// ComboBox related items
    void on_combo_changed();
    void initComboBox();
    void fillupList();
    void resetList();

    class ModelColumns : public Gtk::TreeModel::ColumnRecord
    {
        public:
        ModelColumns() { add(col_id); add(col_name); }
        
        //Tree model columns
        Gtk::TreeModelColumn<int> col_id;
        Gtk::TreeModelColumn<Glib::ustring> col_name;
    };

    ModelColumns mColumns;
    Gtk::ComboBox mCombo;
    Glib::RefPtr<Gtk::ListStore> mRefTreeModel;
};

#endif
