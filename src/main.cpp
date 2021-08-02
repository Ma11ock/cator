#include "main.hpp"

#include <wx-3.0/wx/artprov.h>
#include <wx-3.0/wx/wfstream.h>

BEGIN_EVENT_TABLE(Main, wxFrame)
EVT_MENU(wxID_NEW, Main::OnNew)
EVT_MENU(wxID_EXIT, Main::OnQuit)
EVT_MENU(ID_FireEvent, Main::OnFireEvent)
EVT_MENU(ID_InsertFile, Main::InsertFile)
EVT_MENU(ID_VisitFile, Main::VisitNewFile)
EVT_MENU(wxID_ABOUT, Main::OnAbout)

EVT_MENU(ID_Undo, Main::Undo)
EVT_MENU(ID_Redo, Main::Redo)

EVT_MENU(ID_UseSystemFontToggle, Main::ToggleSystemFont)

EVT_MENU(ID_ForwardSearch, Main::ForwardSearch)

EVT_MENU(ID_Describe, Main::Describe)

EVT_TOOL(wxID_HELP, Main::OnHelp)
EVT_SAVBUF(Main::OnBufferSave)
END_EVENT_TABLE()

Main::Main()
: wxFrame(nullptr, wxID_ANY, "Cator", {}, {800, 600})
{
    /* Create the top (global) menu bar. */
    auto menuBar = new wxMenuBar;

    auto file = new wxMenu;
    file->Append(ID_VisitFile, wxT("&Visit New File"));
    file->Append(ID_InsertFile, wxT("&Insert File"));
    file->Append(ID_FireEvent, wxT("&Save Buffer"));
    file->Append(wxID_EXIT, wxT("&Quit"));
    menuBar->Append(file, wxT("&File"));

    auto edit = new wxMenu;
    edit->Append(ID_Undo, wxT("&Undo"));
    edit->Append(ID_Redo, wxT("&Redo"));
    menuBar->Append(edit, wxT("&Edit"));

    auto options = new wxMenu;
    options->AppendCheckItem(ID_UseSystemFontToggle, wxT("&Use System Font"));
    menuBar->Append(options, wxT("&Options"));

    _bufferMenu = new wxMenu;
    menuBar->Append(_bufferMenu, wxT("&Buffers"));

    auto tools = new wxMenu;
    tools->Append(ID_ForwardSearch, wxT("&Forward Search"));
    menuBar->Append(tools, wxT("&Tools"));

    auto help = new wxMenu;
    help->Append(ID_Describe, wxT("&Describe"));
    menuBar->Append(help, wxT("&Help"));

    _textArea = new wxRichTextCtrl(this, wxID_ANY);

    SetMenuBar(menuBar);
}

void Main::OnNew(wxCommandEvent &event)
{
    wxMessageBox("MainWindow::onNew");
}

void Main::OnQuit(wxCommandEvent &event)
{
    wxMessageBox("MainWindow::onQuit");
}

void Main::OnHelp(wxCommandEvent &event)
{
    wxMessageBox("MainWindow::onHelp");
}

void Main::OnAbout(wxCommandEvent &event)
{
    wxMessageBox("MainWindow::About");
}

void Main::OnBufferSave(SaveBufferEvent &event)
{
    wxMessageBox("It works!");
}

void Main::OnFireEvent(wxCommandEvent &event)
{
    SaveBufferEvent newEvent(this);

    if(ProcessEvent(newEvent) == false)
        wxLogWarning("Could not process event!");
}

void Main::InsertFile(wxCommandEvent &event)
{
    wxMessageBox("Insert a file!");
}

void Main::VisitNewFile(wxCommandEvent &event)
{
    wxFileDialog openFileDialog(this, wxT("Choose a file"), "", "",
                                "*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if(openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    if(!_textArea->LoadFile(openFileDialog.GetPath()))
    {
        wxLogError("Cannot open file \"%s\"", openFileDialog.GetPath());
        return;
    }

}

void Main::Undo(wxCommandEvent &event)
{
    wxMessageBox("Undo.");
}

void Main::Redo(wxCommandEvent &event)
{
    wxMessageBox("Redo.");
}


void Main::ToggleSystemFont(wxCommandEvent &event)
{
    wxMessageBox("Toggle");
}

void Main::ForwardSearch(wxCommandEvent &event)
{
    wxMessageBox("ForwardSearch");
}

void Main::Describe(wxCommandEvent &event)
{
    wxMessageBox("Help!");
}

Main::~Main()
{
    
}
