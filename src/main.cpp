#include "main.hpp"

#include <wx-3.0/wx/artprov.h>
#include <wx-3.0/wx/wfstream.h>
#include <cstring>

#include <filesystem>
#include <string>
#include "util.hpp"

#include <cstdlib>
#include <cstdio>
#include <fstream>

extern "C"
{
#include <linux/limits.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
}

using namespace std::string_literals;

namespace fs = std::filesystem;

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
EVT_MENU(ID_License, Main::ShowLicense)

EVT_MENU(ID_Copy, Main::Copy)
EVT_MENU(ID_Paste, Main::Paste)
EVT_MENU(ID_Cut, Main::Cut)

EVT_MENU(ID_Open, Main::Open)
EVT_MENU(ID_OpenDir, Main::OpenDir)

EVT_TOOL(wxID_HELP, Main::OnHelp)
EVT_SAVBUF(Main::OnBufferSave)
END_EVENT_TABLE()

Main::Main()
: wxFrame(nullptr, wxID_ANY, "Cato", {}, {800, 600})
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
    help->Append(ID_License, wxT("&License"));
    menuBar->Append(help, wxT("&Help"));


    SetMenuBar(menuBar);

    auto toolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition,
                                 wxDefaultSize, wxTB_HORIZONTAL | wxTB_NOICONS);
    toolBar->AddTool(ID_Cut, wxT("Cut"), wxNullBitmap);
    toolBar->AddTool(ID_VisitFile, wxT("New File"), wxNullBitmap);
    toolBar->AddTool(ID_Open, wxT("Open"), wxNullBitmap);
    toolBar->AddTool(ID_OpenDir, wxT("Open Directory"), wxNullBitmap);

    toolBar->AddTool(ID_Cut, wxT("Cut"), wxNullBitmap);
    toolBar->AddTool(ID_Copy, wxT("Copy"), wxNullBitmap);
    toolBar->AddTool(ID_Paste, wxT("Paste"), wxNullBitmap);
    toolBar->AddTool(ID_Undo, wxT("Undo"), wxNullBitmap);
    toolBar->AddTool(ID_Redo, wxT("Redo"), wxNullBitmap);

    toolBar->AddTool(wxID_EXIT, wxT("Quit"), wxNullBitmap);


    SetToolBar(toolBar);

    _textArea = new wxRichTextCtrl(this, wxID_ANY);

    _statusBar = new wxStatusBar(this);
    _statusBar->SetFieldsCount(2);
    _statusBar->SetStatusText("(Unnamed)", 0);
    _statusBar->SetStatusText("Col 0 Row 0", 1);
    SetStatusBar(_statusBar);

    _textArea->Bind(wxEVT_RICHTEXT_CHARACTER, &Main::KeyDown, this);
    _textArea->Bind(wxEVT_RICHTEXT_DELETE, &Main::KeyDown, this);
    _textArea->Bind(wxEVT_RICHTEXT_RETURN, &Main::KeyDown, this);
    _textArea->Bind(wxEVT_RICHTEXT_LEFT_CLICK, &Main::KeyDown, this);
    _textArea->Bind(wxEVT_RICHTEXT_CONTENT_INSERTED, &Main::KeyDown, this);
    _textArea->Bind(wxEVT_RICHTEXT_CONTENT_DELETED, &Main::KeyDown, this);
    _textArea->Bind(wxEVT_RICHTEXT_BUFFER_RESET, &Main::KeyDown, this);
    _textArea->Bind(wxEVT_RICHTEXT_SELECTION_CHANGED, &Main::KeyDown, this);

    _isDired = false;

    _userID = geteuid();
    passwd *pw = getpwuid(_userID);
    if(pw)
        _userName = pw->pw_name;
    else
        _userName = "Unknown";

    wxInitAllImageHandlers();
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
    wxMessageBox(R"ABOUT(
Cator is a simple, emacs-like and joe-like text editor.
It can also edit directories.
)ABOUT");
}

void Main::OnBufferSave(SaveBufferEvent &event)
{
    if(_isDired)
    {
        auto linNum = _textArea->GetNumberOfLines();
        for(auto i = 0; i < linNum; i++)
        {
            if(i >= _dirFileList.size())
                break;

            std::string line = _textArea->GetLineText(i).ToStdString();
            std::string fileName = cator::tailUntil(line, ' ');
            if(fileName == line)
                continue;
            auto oldFileName = cator::tailUntil(_dirFileList[i], ' ');
            fs::path oldPath = fs::path(get_current_dir_name()) / oldFileName;
            /* First, check the file name. If it's different, delete the file. */
            if(oldFileName != fileName)
            {
                fs::path newPath = fs::path(get_current_dir_name()) / fileName;
                std::error_code er;
                if(fs::rename(oldPath, newPath, er);
                   er != std::error_code())
                {
                    auto errMsg = cator::sprintf("Could not rename file \"%s\" "
                                                 ": %s",
                                                 oldPath.generic_string(),
                                                 cator::errStr(er));
                    wxMessageBox(wxString(errMsg), "File Rename Error",
                                 wxICON_ERROR);
                    std::cerr << "File rename error: " << errMsg << '\n';
                }
            }
            /* Check for change in file permissions. */
            for(auto j = 1; j < 10; j++)
            {
                auto add = fs::perm_options::add;
                std::error_code err;
                auto oldPerms = _dirFileList[i];
                if(line[j] != oldPerms[j])
                {
                    switch(line[j])
                    {
                    case 'r':
                        if(j < 3)
                            fs::permissions(oldPath, fs::perms::owner_read,
                                            add, err);
                        else if(j < 6)
                            fs::permissions(oldPath, fs::perms::group_read,
                                            add, err);
                        else
                            fs::permissions(oldPath, fs::perms::others_read,
                                            add, err);
                        break;

                    case 'w':
                        if(j < 3)
                            fs::permissions(oldPath, fs::perms::owner_write,
                                            add, err);
                        else if(j < 6)
                            fs::permissions(oldPath, fs::perms::group_write,
                                            add, err);
                        else
                            fs::permissions(oldPath, fs::perms::others_write,
                                            add, err);
                        break;


                    case 'x':
                        if(j < 3)
                            fs::permissions(oldPath, fs::perms::owner_exec,
                                            add, err);
                        else if(j < 6)
                            fs::permissions(oldPath, fs::perms::group_exec,
                                            add, err);
                        else
                            fs::permissions(oldPath, fs::perms::others_exec,
                                            add, err);
                        break;


                    default:
                        wxMessageBox(wxString(cator::sprintf("Could not understand the meaning of %c",
                                                             line[i])),
                                     "Bad argument for file permissions",
                                     wxICON_ERROR);
                        break;
                    }
                    if(err != std::error_code())
                    {
                        auto errMsg = cator::sprintf("Could not change file permissions for \"%s\" "
                                                     ": %s",
                                                     oldPath.generic_string(),
                                                     cator::errStr(err));
                        wxMessageBox(wxString(errMsg), "File Permission Error",
                                     wxICON_ERROR);
                        std::cerr << "File Permission error: " << errMsg << '\n';
                    }
                }
            }
            /* Check for change in file owner. */
            if(line.substr(13, _userName.size()) != _userName &&
               _userName != "Unkown")
            {
                std::string newUser = cator::substrUntil(line, 13, ' ');
                passwd *p;
                p = getpwnam(newUser.c_str());
                if(p == nullptr)
                {
                    auto errMsg = cator::sprintf("User %s does not exist.",
                                                 newUser);
                    wxMessageBox(wxString(errMsg), "Invalid User",
                                 wxICON_ERROR);
                    std::cerr << "Invalid User: " << errMsg << '\n';
                }

                uid_t newUserID = p->pw_uid;
                std::cout << "The new user is " << newUser << " with id " << newUserID << '\n';

                if(chown(oldPath.generic_string().c_str(), newUserID, 0) != 0)
                {
                    auto errMsg = cator::sprintf("Could not change onwer for \"%s\" "
                                                 "from %s to %s: %s",
                                                 oldPath.generic_string(),
                                                 _userName,
                                                 newUser,
                                                 cator::errStr());
                    wxMessageBox(wxString(errMsg), "File Chown Error",
                                 wxICON_ERROR);
                    std::cerr << "File Chown error: " << errMsg << '\n';
                }
            }

        }
    }
    else
        _textArea->SaveFile();
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
                                "*", wxFD_SAVE);

    if(openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    /* If the file does exist, create it. */
    if(!fs::exists(openFileDialog.GetPath().ToStdString()))
    {
        wxFile tmp;
        tmp.Create(openFileDialog.GetPath());
        tmp.Close();
    }
    if(!_textArea->LoadFile(openFileDialog.GetPath()))
    {
        _textArea->SetFilename(openFileDialog.GetPath());
        wxLogError("Cannot open file \"%s\"", openFileDialog.GetPath());
        return;
    }

    _statusBar->SetStatusText(openFileDialog.GetPath(), 0);
}

void Main::Undo(wxCommandEvent &event)
{
    _textArea->Undo();
}

void Main::Redo(wxCommandEvent &event)
{
    _textArea->Redo();
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
    wxMessageBox(R"HELP(
This is the directory editor.
You can delete a file by erasing its line in this buffer.
You can change a file's permissions by altering its the first 10 characters.
You can change the owner and creator by altering its entry.
Ditto with the file's name.
)HELP");
}

std::string loadGPL()
{
    std::ifstream gpl("../LICENSE");
    std::string gplStr((std::istreambuf_iterator<char>(gpl)),
                       std::istreambuf_iterator<char>());
    return gplStr;
}

void Main::ShowLicense(wxCommandEvent &event)
{
    static bool hasLicense = false;
    static std::string gpl;
    if(!hasLicense)
    {
        gpl = loadGPL();
        hasLicense = true;
    }
    wxMessageBox(gpl);
}

void Main::Copy(wxCommandEvent &event)
{
    _textArea->Copy();
}

void Main::Paste(wxCommandEvent &event)
{
    _textArea->Paste();
}

void Main::Cut(wxCommandEvent &event)
{
    _textArea->Cut();
}



void Main::KeyDown(wxRichTextEvent &event)
{
    long x;
    long y;
    _textArea->PositionToXY(event.GetPosition(), &x, &y);
    _statusBar->SetStatusText(cator::sprintf("Col %ld Row %ld", x, y), 1);
}


void Main::Open(wxCommandEvent &event)
{
    wxFileDialog openFileDialog(this, wxT("Choose a file"), "", "",
                                "*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if(openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    if(!fs::exists(openFileDialog.GetPath().ToStdString()))
    {
        wxFile tmp;
        tmp.Create(openFileDialog.GetPath());
        tmp.Close();
    }

    if(!_textArea->LoadFile(openFileDialog.GetPath()))
    {
        _textArea->SetFilename(openFileDialog.GetPath());
        wxLogError("Cannot open file \"%s\"", openFileDialog.GetPath());
        return;
    }

    _statusBar->SetStatusText(openFileDialog.GetPath(), 0);
}

void Main::OpenDir(wxCommandEvent &event)
{
    wxDirDialog dlg(this, "Choose a directory to edit", "",
        wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST | wxDD_CHANGE_DIR);

    if(dlg.ShowModal() == wxID_CANCEL)
        return;

    char buf[PATH_MAX];
    FILE *ls = popen(cator::sprintf("/bin/ls -l %s", dlg.GetPath().ToStdString()).c_str(),
        "r");
    if(ls == nullptr)
    {
        std::cerr << "Could not ls the file " << dlg.GetPath().ToStdString()
                  << '\n';
        return;
    }

    _textArea->SetFilename("");
    _textArea->Clear();
    _dirFileList.clear();
    _isDired = true;
    bool isFirstLine = true;

    while(fgets(buf, sizeof(buf), ls) != nullptr)
    {
        /* Ignore \n at the end. */
        std::string trunc(buf, strlen(buf) - 1);
        std::string fileName = cator::tailUntil(trunc,' ');
        /* Skip the first line (length). */
        if(isFirstLine)
        {
            isFirstLine = false;
            continue;
        }

        if(cator::strEnd(fileName, ".png"))
        {
            if(!_textArea->WriteImage(wxString(fileName), wxBITMAP_TYPE_PNG))
            {
                std::cout << "Could not write image " << fileName << '\n';
            }
        }
        _textArea->WriteText(wxString(buf));
        _dirFileList.push_back(trunc);
    }

    _statusBar->SetStatusText(dlg.GetPath(), 0);
}

