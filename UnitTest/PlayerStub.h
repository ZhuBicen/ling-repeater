#ifndef PLAYER__STUB__H
#define PLAYER__STUB__H
#include <windows.h>

class Player_impl
{
public:
    Player_impl();
    ~Player_impl();

    void Init(HWND, HINSTANCE);

    bool OpenFile(const std::wstring& fileName);
    bool CloseFile();
    bool PlayFile();

    std::wstring GetFileName();
    long GetFileLength();

    bool PlayFrom(long pos);
    bool ContinuePlay();
 
    bool Repeat();

    bool SetStartPos();
    bool Resume();
    bool Pause();
    bool Stop();
    const PlayerState& GetState();

private:
    PlayerState state_;
};
#endif
