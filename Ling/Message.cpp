#include "Precompiled.hpp"
#include "Message.hpp"
#include <iostream>

std::ostream& operator<< (std::ostream& os, const OpenFileEvent& evt){
    os << " OpenFileEvent (file_name = ";
    const wchar_t* old_file_name = evt.file_name.c_str();
    char new_file_name[MAX_PATH];
    WideCharToMultiByte(CP_ACP, NULL, old_file_name, -1, new_file_name, MAX_PATH, NULL, NULL);
    os <<  new_file_name ;
    return os;
}
std::ostream& operator<< (std::ostream& os, const Event& evt){
    return os << "#"<< messageIdToString(evt.Id());
}
#define caseEventId(x) case x: return std::string(#x)
std::string messageIdToString(int eventId){
    switch (eventId){
        caseEventId(EVENT_ID_OPEN_FILE);
        caseEventId(EVENT_ID_UPDATE_POS);
        caseEventId(EVENT_ID_SET_START_POS);
        caseEventId(EVENT_ID_SET_END_POS);
        caseEventId(EVENT_ID_STOP_FILE);
        caseEventId(EVENT_ID_CONTINUE_PLAY);
        caseEventId(EVENT_ID_START_REPEAT);
        caseEventId(EVENT_ID_SET_POS);
        caseEventId(EVENT_ID_PAUSE_RESUME);
        caseEventId(EVENT_ID_TERMINATE);
        caseEventId(EVENT_ID_PLAYING_FINISHED);
        caseEventId(EVENT_ID_SAVE_SECTION);
        caseEventId(EVENT_ID_REQUEST_PAINT_INFO);
        caseEventId(EVENT_ID_REQUEST_CONTEXT_MENU_INFO);
    }
    return "Unkonwn Message id";
}