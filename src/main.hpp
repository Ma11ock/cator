#ifndef CATOR_MAIN_HPP
#define CATOR_MAIN_HPP

#include <wx-3.0/wx/wx.h>
#include <wx-3.0/wx/richtext/richtextctrl.h>

#include "saveBuf.hpp"

class Main : public wxFrame
{
private:

    wxMenu *_bufferMenu;
    wxRichTextCtrl *_textArea;

    void OnNew(wxCommandEvent &event);
    void OnQuit(wxCommandEvent &event);
    void OnHelp(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);
    void OnBufferSave(SaveBufferEvent &event);
    void OnFireEvent(wxCommandEvent &event);
    void InsertFile(wxCommandEvent &event);
    void VisitNewFile(wxCommandEvent &event);
    void Undo(wxCommandEvent &event);
    void Redo(wxCommandEvent &event);
    void ToggleSystemFont(wxCommandEvent &event);
    void ForwardSearch(wxCommandEvent &event);
    void Describe(wxCommandEvent &event);
public:
    Main();
    ~Main();

    DECLARE_EVENT_TABLE()
};

enum
{
	ID_Quit = 1,
	ID_FireEvent,
    ID_InsertFile,
    ID_VisitFile,
    ID_Undo,
    ID_Redo,
    ID_UseSystemFontToggle,
    ID_ForwardSearch,
    ID_Describe
};

#endif /* CATOR_MAIN_HPP */
