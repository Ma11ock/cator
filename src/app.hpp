#ifndef CATOR_APP_HPP
#define CATOR_APP_HPP

#include <wx-3.0/wx/wx.h>
#include <wx-3.0/wx/menu.h>

#include "main.hpp"

class App : public wxApp
{
private:
    Main *_frame;
public:
    App();
    ~App() = default;
    bool OnInit();
    int FilterEvent(wxEvent &event);
};

#endif /* CATOR_APP_HPP */
