#ifndef CATOR_MAIN_HPP
#define CATOR_MAIN_HPP

#include <wx-3.0/wx/wx.h>

#include "saveBuf.hpp"

class Main : public wxFrame
{
private:
    void OnNew(wxCommandEvent &event);
    void OnQuit(wxCommandEvent &event);
    void OnHelp(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);
    void OnBufferSave(SaveBufferEvent &event);
    void OnFireEvent(wxCommandEvent &event);
public:
    Main();
    ~Main();

    DECLARE_EVENT_TABLE()
};

enum
{
	ID_Quit = 1,
	ID_FireEvent,
};

#endif /* CATOR_MAIN_HPP */
