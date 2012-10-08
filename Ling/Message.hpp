#ifndef LING_MESSAGE_HPP
#define LING_MESSAGE_HPP
#include "StateChart.hpp"
#include <iostream>
#include <string>


enum {
    EVENT_ID_OPEN_FILE = 0,
    EVENT_ID_UPDATE_POS,
    EVENT_ID_SET_START_POS,
    EVENT_ID_SET_END_POS,
    EVENT_ID_STOP_FILE,
    EVENT_ID_CONTINUE_PLAY,
    EVENT_ID_START_REPEAT,
    EVENT_ID_SET_POS,
    EVENT_ID_PAUSE_RESUME,
    EVENT_ID_TERMINATE,
    EVENT_ID_PLAYING_FINISHED,
    EVENT_ID_SAVE_SECTION,
    EVENT_ID_REQUEST_PAINT_INFO,
    EVENT_ID_REQUEST_CONTEXT_MENU_INFO
};

struct OpenFileEvent{
    const std::wstring file_name;
    explicit OpenFileEvent(const std::wstring p_file_name):file_name(p_file_name){}
};

struct SetPosEvent{
    long pos;
    explicit SetPosEvent(long p):pos(p){}
};

struct RequestContextMenuInfoEvent{
    long pos_;
    int x_;
    int y_;
    RequestContextMenuInfoEvent(long pos, long x, long y): pos_(pos), x_(x), y_(y){}
};

class EventFactory {
public:
    static Event* makeEvent(int event_id){
        return new Event(event_id, 0, 0);
    }
    static Event* makeOpenFileEvent(std::wstring filename) {
        void* data = malloc(sizeof(OpenFileEvent));
        OpenFileEvent* ofe = new(data) OpenFileEvent(filename);
        return new Event(EVENT_ID_OPEN_FILE, ofe, sizeof(OpenFileEvent));
    }
    static Event* makeSetPosEvent(long pos) {
        void* data = malloc(sizeof(SetPosEvent));
        SetPosEvent* spe = new(data) SetPosEvent(pos);
        return new Event(EVENT_ID_SET_POS, spe, sizeof(SetPosEvent));
    }

    static Event* makeRequestContextMenuInfoEvent(long pos, long x, long y) {
        void* data = malloc(sizeof(RequestContextMenuInfoEvent));
        RequestContextMenuInfoEvent* e = new (data) RequestContextMenuInfoEvent(pos, x, y);
        return new Event(EVENT_ID_REQUEST_CONTEXT_MENU_INFO, e, sizeof(RequestContextMenuInfoEvent));
    }
};
    
#endif
