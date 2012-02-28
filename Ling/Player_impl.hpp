#ifndef PLAYER__IMPL__VFW__H
#define PLAYER__IMPL__VFW__H

#include <windows.h>
#include <TCHAR.h>
#include <libzplay.h>
using namespace libZPlay;
class MessageQueue;
class Player_impl
{
public:
    Player_impl(MessageQueue* mq);
    ~Player_impl();

    bool OpenFile(const std::wstring& fileName);
    bool CloseFile();
    bool Play();
    bool PlayFromTo(long start_pos, long end_pos);
    bool PlayFrom(long pos);
    
    long GetFileLength();
    long GetCurrentPos();
    
    bool Resume();
    bool Pause();
    bool Stop();

private:
    ZPlay *player_; 
    MessageQueue* mq_;
};
#endif