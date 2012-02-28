#ifndef PLAYER__H
#define PLAYER__H


class PlayerState;
class MessageQueue;
namespace libZPlay{
    class ZPlay;
}

class Player{
public:

    Player(MessageQueue* mq);
    ~Player();
    bool OpenFile(const std::wstring& file_name);
    bool CloseFile();
    
    bool Play();
    bool Play(long start_pos, long end_pos = 0);
    bool Seek(long pos);

    //bool PlayFrom(long pos);
    
    long GetFileLength();
    long GetCurrentPos();

    bool Pause();
    bool Resume();
    bool Stop();
    
    bool IsPlaying();

private:
    libZPlay::ZPlay* player_;
    MessageQueue* mq_;
};

#endif
