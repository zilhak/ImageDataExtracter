#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/listctrl.h>
#include <wx/filename.h>

#include <ui/component/ImagePanel.hpp>
#include <ui/component/FileExplorer.hpp>
#include <ui/detection/DetectionInfoBox.hpp>
#include <ui/detection/DetectionToolBar.hpp>

#include <set>
#include <vector>
#include <algorithm>


class DetectionFrame : public wxDialog
{
public:
    wxDECLARE_EVENT_TABLE();

public:
    enum ButtonID {
        OPEN,
        SETTING
    };
    enum ID {
        COMBO_SIZE,
        COMBO_TYPE,
        COMBO_SCALE,
        COMBO_DIFFICULT,
        IMAGE_VIEWER,
        FILE_EXPLORER
    };
    enum STATUS {
        IDLE,

    };

private:
    wxString _dir;
    wxString _current_file;
    std::set<wxString> _image_extension;
    std::vector<wxString> _folder_name_list;

private:
    FileExplorer * _file_list;
    DetectionToolBar * _toolbar;
    DetectionInfoBox * _infobox;
    ImagePanel * _image_panel;

private:
    wxComboBox * _type_combobox;
    wxComboBox * _scale_combobox;
    wxComboBox * _difficult_combobox;

public:
    DetectionFrame(wxString const & title);
    ~DetectionFrame();

private:
    void initialize();
    void initializeStyle();
    void initializeSetting();
    void initializeToolBar(wxBoxSizer * sizer);
    void initializeLeftMenu(wxBoxSizer * h_sizer);
    void initializeImageViewer(wxBoxSizer * h_sizer);

private: // menu event.
    void onZoomBox(wxCommandEvent &event);
    void onTypeComboBox(wxCommandEvent &event);
    void onScaleComboBox(wxCommandEvent &event);
    void onDifficultComboBox(wxCommandEvent &event);
    void onOpenButton(wxCommandEvent & event);
    void onSelectFile(wxListEvent & event);
    void onExplore();

private: // input event.
    void onMouseEvent(wxMouseEvent & event);
    void onKeyboardEvent(wxKeyEvent & event);

private:
    void prev();
    void next();

private:
    void setImagePanel();

private:
    void moveFile(wxString const & folder_name);
};

