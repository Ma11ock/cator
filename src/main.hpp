#ifndef CATOR_MAIN_HPP
#define CATOR_MAIN_HPP

#include <wx-3.0/wx/wx.h>
#include <wx-3.0/wx/richtext/richtextctrl.h>
#include <wx-3.0/wx/event.h>
#include <string>
#include <vector>

#include "saveBuf.hpp"

class Main : public wxFrame
{
private:
    std::vector<std::string> _dirFileList;
    wxMenu *_bufferMenu;
    wxRichTextCtrl *_textArea;
    wxStatusBar *_statusBar;
    bool _isDired;
    std::string _userName;
    uid_t _userID;

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
    void ShowLicense(wxCommandEvent &event);
    void Copy(wxCommandEvent &event);
    void Paste(wxCommandEvent &event);
    void Cut(wxCommandEvent &event);
    void Open(wxCommandEvent &event);
    void OpenDir(wxCommandEvent &event);

    void ChangeFilePermission();
    void ChangeFileOwner();
    void MaybeDeleteFile();
public:
    Main();
    ~Main() = default;

    void KeyDown(wxRichTextEvent &event);

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
    ID_Describe,
    ID_License,
    ID_Open,
    ID_OpenDir,
    ID_Copy,
    ID_Paste,
    ID_Cut,
};

#endif /* CATOR_MAIN_HPP */
