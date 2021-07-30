#include "app.hpp"

wxIMPLEMENT_APP(App);

App::App()
    :_frame(nullptr)
{
}

bool App::OnInit()
{
    std::cout << "Init!\n";
    _frame = new Main();
    _frame->Show();
    return true;
}
