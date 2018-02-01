
#include "frame.hpp"
#include <iostream>

wxBEGIN_EVENT_TABLE(Frame, wxFrame)
    EVT_BUTTON(ID::BUTTON_OPEN, Frame::onOpenButton)
    EVT_CHAR_HOOK(Frame::onKeyboardEvent)
    EVT_MOUSE_EVENTS(Frame::onMouseEvent)
    EVT_COMBOBOX(ID::COMBO_SIZE, Frame::onSizeComboBox)
    EVT_COMBOBOX(ID::COMBO_TYPE, Frame::onTypeComboBox)
    EVT_COMBOBOX(ID::COMBO_SCALE, Frame::onScaleComboBox)
    EVT_COMBOBOX(ID::COMBO_DIFFICULT, Frame::onDifficultComboBox)
    EVT_LIST_ITEM_ACTIVATED(wxID_ANY, Frame::onListDoubleClick)
wxEND_EVENT_TABLE()

constexpr static char const * const TRASHBIN_NAME = "TrashCan";

Frame::Frame(const wxString & title) : wxFrame(NULL, wxID_ANY, title)
{
    initialize();
}

Frame::~Frame()
{
    /*EMPTY*/
}

void Frame::initialize()
{
    wxBoxSizer * v_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer * h_sizer = new wxBoxSizer(wxHORIZONTAL);

    initializeStyle();
    initializeSetting();
    initializeToolBar(v_sizer);

    initializeLeftMenu(h_sizer);
    initializeImageViewer(h_sizer);

    v_sizer->Add(h_sizer, 1, wxEXPAND);
    SetSizer(v_sizer);

    Refresh();
}

void Frame::initializeStyle()
{
    SetMinSize(wxSize(400, 300));
    SetSize(wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X),
                   wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)));
}

void Frame::initializeSetting()
{
    _image_extension.insert(wxT("jpg"));
    _image_extension.insert(wxT("jpeg"));
    _image_extension.insert(wxT("png"));
}

void Frame::initializeToolBar(wxBoxSizer * sizer)
{
    _tool_bar = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(100, 40));
    wxBoxSizer * h_sizer = new wxBoxSizer(wxHORIZONTAL);
    _open_button = new wxButton(_tool_bar, ID::BUTTON_OPEN, wxT("Open"));
    _close_button = new wxButton(_tool_bar, ID::BUTTON_XML, wxT("XML view"));

    _size_combobox = new wxComboBox(_tool_bar, ID::COMBO_SIZE, wxT("100"));
    _size_combobox->SetEditable(false);
    for (int i = 10; i <= 600; i += 10) {
        _size_combobox->Append(wxString::Format("%d", i));
    }

    _type_combobox = new wxComboBox(_tool_bar, ID::COMBO_TYPE, wxT("car"));
    _type_combobox->SetEditable(false);
    _type_combobox->Append(wxT("car"));
    _type_combobox->Append(wxT("truck"));
    _type_combobox->Append(wxT("bus"));
    _type_combobox->Append(wxT("person"));

    _scale_combobox = new wxComboBox(_tool_bar, ID::COMBO_SCALE, wxT("3"));
    _scale_combobox->SetEditable(false);
    _scale_combobox->Append(wxT("1"));
    _scale_combobox->Append(wxT("2"));
    _scale_combobox->Append(wxT("3"));

    _difficult_combobox = new wxComboBox(_tool_bar, ID::COMBO_DIFFICULT, wxT("1"));
    _difficult_combobox->Append(wxT("0"));
    _difficult_combobox->Append(wxT("1"));
    _difficult_combobox->Append(wxT("2"));

    h_sizer->Add(_open_button, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
    h_sizer->Add(_close_button, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 15);
    h_sizer->Add(new wxStaticText(_tool_bar, wxID_ANY, "Size:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    h_sizer->Add(_size_combobox, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 15);
    h_sizer->Add(new wxStaticText(_tool_bar, wxID_ANY, "Type:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    h_sizer->Add(_type_combobox, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 15);
    h_sizer->Add(new wxStaticText(_tool_bar, wxID_ANY, "Scale:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    h_sizer->Add(_scale_combobox, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 15);
    h_sizer->Add(new wxStaticText(_tool_bar, wxID_ANY, "Difficult:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    h_sizer->Add(_difficult_combobox, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 15);

    _tool_bar->SetSizer(h_sizer);

    sizer->Add(_tool_bar, 0, wxEXPAND);
}

void Frame::initializeLeftMenu(wxBoxSizer *h_sizer)
{
    wxBoxSizer * v_sizer = new wxBoxSizer(wxVERTICAL);

    _file_list_viewer = new FileExplorer(this);
    _file_list_viewer->SetBackgroundColour(wxColour(0xCCCCCC));

    _image_info_box = new wxPanel(this, wxID_ANY);
    _image_info_box->SetBackgroundColour(0xBBBBBB);
    wxGridSizer * panel_sizer = new wxGridSizer(2, 3, 5);
    _image_info_box->SetSizer(panel_sizer);

    _info_image_name = new wxStaticText(_image_info_box, wxID_ANY, "");
    _info_image_size = new wxStaticText(_image_info_box, wxID_ANY, "");
    _info_mouse_x = new wxStaticText(_image_info_box, wxID_ANY, "");
    _info_mouse_y = new wxStaticText(_image_info_box, wxID_ANY, "");

    panel_sizer->Add(new wxStaticText(_image_info_box, wxID_ANY, "ImageName :"));
    panel_sizer->Add(_info_image_name);
    panel_sizer->Add(new wxStaticText(_image_info_box, wxID_ANY, "ImageSize :"));
    panel_sizer->Add(_info_image_size);
    panel_sizer->Add(new wxStaticText(_image_info_box, wxID_ANY, "MouseX :"));
    panel_sizer->Add(_info_mouse_x);
    panel_sizer->Add(new wxStaticText(_image_info_box, wxID_ANY, "MouseY :"));
    panel_sizer->Add(_info_mouse_y);

    v_sizer->Add(_image_info_box, 1, wxEXPAND);
    v_sizer->Add(_file_list_viewer, 5, wxEXPAND);

    h_sizer->Add(v_sizer, 0, wxEXPAND);
}

void Frame::initializeImageViewer(wxBoxSizer * sizer)
{
    _image_viewer = new ImagePanel(this, wxID_ANY);
    _image_viewer->SetBackgroundColour(wxColour(0xFFFFFF));

    sizer->Add(_image_viewer, 1, wxEXPAND);
}

bool calculateRate(int image_width, int image_height)
{
//    bool result = false;
//    int screen_x = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
//    int screen_y = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);

//    if (image_width > screen_x) {
//        result_x = screen_x - 50;
//        x_rate = static_cast<double>(image_width) / static_cast<double>(screen_x - 50);
//        result = true;
//    } else {
//        result_x = image_width;
//    }
//    if (image_height > screen_y) {
//        result_y = screen_y - 100;
//        y_rate = static_cast<double>(image_height) / static_cast<double>(screen_y-100);
//        result = true;
//    } else {
//        result_y = image_height;
//    }

    return true;
}

bool Frame::fileExtCheck(wxString const extension)
{
    if (_image_extension.find(extension) != _image_extension.end()) {
        return true;
    } else {
        return false;
    }
}

void Frame::onOpenButton(wxCommandEvent & event)
{
     wxFileDialog * find = new wxFileDialog(this,
                                            _("Open jpg file"),
                                            "",
                                            "",
                                            "JPG files (*.jpg;*.jpeg)|*.jpg;*.jpeg");
     if (find->ShowModal() == wxID_CANCEL) {
         return;
     }
     makeFileList(find->GetDirectory());
    for (auto file : _file_list) {
        if (file == find->GetFilename()) {
            showImage(file);
            return;
        }
    }
    std::cout << "Cannot Found file!!!" << std::endl;
    std::cout << "Tip: if path name have korean, this error occur." << std::endl;
}

void Frame::makeFileList(wxString const directory)
{
    _file_list_viewer->DeleteAllItems();
    _file_list.clear();

    _dir = new wxDir(directory);

    _dir->GetFirst(&_current_file);
    wxFileName file(directory + "/" + _current_file);

    do {
        file.Assign(directory + "/" + _current_file);
        if (fileExtCheck(file.GetExt())) {
            _file_list.push_back(_current_file);
        }
    } while (_dir->GetNext((&_current_file)));

    std::sort(_file_list.begin(), _file_list.end());

    for (int index = 0; index < _file_list.size(); index++) {
        file.Assign(directory + "/" + _file_list.at(index));
       _file_list_viewer->InsertItem(index, _file_list.at(index));
       if (wxFileExists(directory + "/" + file.GetName() + ".xml")) {
           _file_list_viewer->SetItem(index, 1, ("O"));
       } else {
           _file_list_viewer->SetItem(index, 1, ("X"));
       }
    }
}

void Frame::onListDoubleClick(wxListEvent & event)
{
    showImage(event.GetLabel());
}

void Frame::onKeyboardEvent(wxKeyEvent & event)
{
    std::cout << event.GetKeyCode() << std::endl;
    if (_image_viewer->isReady()) {
        if (event.GetKeyCode() == 69) { // 'e'
            _file_list_viewer->xmlCheck(_current_file);
            _image_viewer->save();
            nextFile();
        } else if (event.GetKeyCode() == 81) { //'q'
            prevFile();
        } else if (event.GetKeyCode() == 87) { //'w'

        } else if (event.GetKeyCode() == 83) { //'s'

        } else if (event.GetKeyCode() == 65) { //'a'

        } else if (event.GetKeyCode() == 68) { //'d'

        } else if (event.GetKeyCode() == 82) { //'r'
            nextFile();
        } else if (event.GetKeyCode() == 49) { //'1'
            _type_combobox->SetValue(wxT("car"));
        } else if (event.GetKeyCode() == 50) { //'2'
            _type_combobox->SetValue(wxT("truck"));
        } else if (event.GetKeyCode() == 51) { //'3'
            _type_combobox->SetValue(wxT("bus"));
        } else if (event.GetKeyCode() == 127) { //'delete'
            wxFileName file(_dir->GetName() + "/" + _current_file);
            wxString trashbin_path = file.GetPath(wxPATH_GET_SEPARATOR) + TRASHBIN_NAME;

            if (!wxDirExists(trashbin_path)) {
                wxMkdir(trashbin_path);
            }
            wxString duplicate_number = "";
            int number = 0;
            while (!wxRenameFile(file.GetFullPath(),
                                 trashbin_path + "/" + file.GetName() + duplicate_number + "." + file.GetExt(),
                                 false)) {
                number++;
                duplicate_number = wxString::Format("(%d)", number);
            }
            if (wxFileExists(file.GetPath(wxPATH_GET_SEPARATOR) + file.GetName() + ".xml")) {
                wxRenameFile(file.GetPath(wxPATH_GET_SEPARATOR) + file.GetName() + ".xml",
                        trashbin_path + "/" + file.GetName() + duplicate_number + ".xml", true);
            }
            makeFileList(file.GetPath());
            refresh();
        }
    }
    SetTitle(_current_file);
}

void Frame::onMouseEvent(wxMouseEvent & event)
{
    int x = static_cast<int>(static_cast<double>(event.GetX() - 15) * 100.0 / wxAtof(_size_combobox->GetValue()));
    int y = static_cast<int>(static_cast<double>(event.GetY() - 15) * 100.0 / wxAtof(_size_combobox->GetValue()));

    _info_mouse_x->SetLabel(wxString::Format("%d", x));
    _info_mouse_y->SetLabel(wxString::Format("%d", y));
}

void Frame::onSizeComboBox(wxCommandEvent &event)
{
    _image_viewer->setSize(static_cast<double>(wxAtoi(_size_combobox->GetValue())) / (double) 100);
    if (_image_viewer->isReady()) {
        _image_viewer->setBackgroundImage(_dir->GetName(), _current_file);
    }
}

void Frame::onTypeComboBox(wxCommandEvent &event)
{
    _image_viewer->setType(_type_combobox->GetValue());
}

void Frame::onScaleComboBox(wxCommandEvent &event)
{
    _image_viewer->setDepth(wxAtoi(_scale_combobox->GetValue()));
}

void Frame::onDifficultComboBox(wxCommandEvent &event)
{
    _image_viewer->setDiff(wxAtoi(_difficult_combobox->GetValue()));
}

void Frame::prevFile()
{
    if (_image_index > 0) {
        _image_index--;
        showImage(_file_list.at(_image_index));
    }
}

void Frame::nextFile()
{
    if (_image_index < _file_list.size() - 1) {
        _image_index++;
        showImage(_file_list.at(_image_index));
    }
}

void Frame::refresh()
{
    if (_image_index < _file_list.size()) {
        showImage(_file_list.at(_image_index));
    } else if (--_image_index < _file_list.size()){
        showImage(_file_list.at(_image_index));
    }
}

void Frame::showImage(wxString const & file_name)
{
    SetTitle(file_name);

    _image_index = _file_list_viewer->highlightItem(file_name);
    _current_file = file_name;
    _image_viewer->setBackgroundImage(_dir->GetName(), _current_file);

    _info_image_name->SetLabel(wxString::Format("%s", _current_file));

    _info_image_size->SetLabel(wxString::Format("%d x %d", _image_viewer->getImageWidth(), _image_viewer->getImageHeight()));
}