#include "app.hpp"

wxIMPLEMENT_APP(App);

App::App()
    :_frame(nullptr)
{
}

bool App::OnInit()
{
    _frame = new Main();
    _frame->Show();


    return true;
}

int App::FilterEvent(wxEvent &event)
{
//    if(event.GetEventType() == wxEVT_KEY_DOWN)
//    {
//        _frame->KeyDown( (wxKeyEvent&)event );
//        return 1;
//    }

    return -1;
}
