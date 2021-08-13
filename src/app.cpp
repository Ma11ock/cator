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
    return -1;
}
