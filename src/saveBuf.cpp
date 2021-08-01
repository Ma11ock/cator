#include "saveBuf.hpp"

IMPLEMENT_DYNAMIC_CLASS(SaveBufferEvent, wxEvent)
DEFINE_EVENT_TYPE(catEVT_SAVEBUF)

SaveBufferEvent::SaveBufferEvent(wxWindow *win)
{
    SetEventType(catEVT_SAVEBUF);
    SetEventObject(win);
}
