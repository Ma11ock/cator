#include "main.hpp"

#include <wx-3.0/wx/artprov.h>

BEGIN_EVENT_TABLE(Main, wxFrame)
EVT_MENU(wxID_NEW, Main::OnNew)
EVT_MENU(wxID_EXIT, Main::OnQuit)
EVT_MENU(ID_FireEvent, Main::OnFireEvent)
EVT_MENU(wxID_ABOUT, Main::OnAbout)
EVT_TOOL(wxID_HELP, Main::OnHelp)
EVT_SAVBUF(Main::OnBufferSave)
END_EVENT_TABLE()

Main::Main()
    : wxFrame(nullptr, wxID_ANY, "Cator", {}, {800, 600})
{
    /* Create the top (global) menubar. */
    auto menuBar = new wxMenuBar;
    auto file = new wxMenu;

    file->Append(ID_FireEvent, wxT("&Save Buffer"));
    file->Append(wxID_EXIT, wxT("&Quit"));
    menuBar->Append(file, wxT("&File"));
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

Main::~Main()
{
    
}
