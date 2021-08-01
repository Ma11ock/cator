#ifndef SAVEBUF_H
#define SAVEBUF_H

#include <wx-3.0/wx/wx.h>

class SaveBufferEvent : public wxEvent
{
public:
    SaveBufferEvent(wxWindow *win = nullptr);
    inline wxEvent *Clone() const { return new SaveBufferEvent (*this); }
    ~SaveBufferEvent() = default;

    DECLARE_DYNAMIC_CLASS(SaveBufferEvent)
};

typedef void (wxEvtHandler::*myEventFunction)(SaveBufferEvent&);

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE(catEVT_SAVEBUF, 1)
END_DECLARE_EVENT_TYPES()

#define EVT_SAVBUF(func)                              \
	DECLARE_EVENT_TABLE_ENTRY(catEVT_SAVEBUF,      \
		-1,                                    \
		-1,                                    \
		(wxObjectEventFunction)                \
		(myEventFunction) & func,              \
		(wxObject *) NULL ),


#endif /* SAVEBUF_H */
