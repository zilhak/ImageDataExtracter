//
// Created by bogonets on 2018. 6. 21..
//

#ifndef FEPSTOOL_FILEMANAGERFRAME_HPP
#define FEPSTOOL_FILEMANAGERFRAME_HPP

#include <wx/wx.h>
#include <ui/filemanager/ManagerControlBox.hpp>
#include <ui/filemanager/ManagerPanel.hpp>
#include <stack>
#include <list>

class FileManagerFrame : public wxDialog
{
private:
    std::vector<wxString> _ext_list;

private:
    std::vector<wxString> _file_list;
    std::vector<bool> _selected_list;
    std::vector<wxString> _deleted_list;

private:
    int _iteration = 0;

private:
    enum BUTTON_ID {
        OPEN
    };

private:
    DECLARE_EVENT_TABLE();

private:
    ManagerControlBox * _control_box;
    ManagerPanel * _screen;

public:
    FileManagerFrame(wxString const & title);
    virtual ~FileManagerFrame();

private:
    void initialize();
    void initializeData();
    void initializeOpen(wxSizer * sizer);
    void initializeLeftPanel(wxSizer * sizer);
    void initializeRightPanel(wxSizer * sizer);

private:
    void onOpen(wxCommandEvent & event);
    void onKeyBoardEvent(wxKeyEvent & event);
    void onMouseEvent(wxMouseEvent & event);

private:
    void next();
    void prev();

private:
    void toggleImage(int index);

private:
    std::vector<wxString> subvector(int iter);
};


#endif //FEPSTOOL_FILEMANAGERFRAME_HPP
