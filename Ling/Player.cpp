#include "Precompiled.hpp"
#include "Player.hpp"

#include <libzplay.h>
#include "MessageQueue.hpp"
using namespace libZPlay;
#pragma comment(lib, "libzplay.lib")

int  __stdcall
ZplayCallbackFunc(void* instance, void *user_data, TCallbackMessage message, 
                  unsigned int param1, unsigned int param2)
{
	ZPlay *myplayer = (ZPlay*) instance;
    MessageQueue* mq = (MessageQueue*)user_data;
	switch(message)
	{
		case MsgStop : 
            mq->PutMessage(boost::shared_ptr<Message>(new PlayFinishedEvent()));
            LOG(logINFO)<< __FUNCTION__ << "<<MsgStop, Emit the Play finished>>"; 
            break;
		return 0;	
	}

	return 0;
}

Player::Player(MessageQueue* mq):mq_(mq){
    player_ = CreateZPlay();
	//no sound card found
	assert(player_->EnumerateWaveOut() != 0);
}
Player::~Player(){
    player_->Release();
}

bool Player::OpenFile(const std::wstring& file_name){
    return player_->OpenFileW(file_name.c_str(), sfAutodetect) != 0;
}

bool Player::CloseFile(){
    return player_->Close() != 0;
}
bool Player::Play(){
    player_->SetCallbackFunc(ZplayCallbackFunc, (TCallbackMessage)(MsgStop) , mq_);    
    return player_->Play() != 0;
}

bool Player::Play(long start_pos, long end_pos)
{
    TStreamTime start_time;
    start_time.ms = start_pos;

    if (end_pos == 0){
        end_pos = GetFileLength();
    }
    else{
        assert(end_pos >= start_pos);
    }

    TStreamTime end_time;
    end_time.ms = end_pos;
    
    int ret = player_->PlayLoop(tfMillisecond, &start_time, tfMillisecond, &end_time, 1, 0);
    player_->SetCallbackFunc(ZplayCallbackFunc, (TCallbackMessage)(MsgStop) , mq_);    
    if( ret == 1){return true;}
    else{
        LOG(logERROR) << "libzplay::PlayLoop failed error info:" << player_->GetError() ;
        return false;
    }
}

bool Player::Seek(long pos)
{
    TStreamTime time;
    time.ms = pos;
    return player_->Seek(tfMillisecond, &time, smFromBeginning ) == 0;
}
long Player::GetFileLength()
{
    TStreamInfoW info;
    player_->GetStreamInfoW(&info);
    return info.Length.ms;
}
long Player::GetCurrentPos()
{
    TStreamTime pos;
    player_->GetPosition(&pos);
    return pos.ms;
}

bool Player::Pause()
{
    return player_->Pause() == 1;
}

bool Player::Resume()
{
    return player_->Resume() == 1;
}
//当我主动的去关掉播放时，比如从重复状态转到正常播放状态，或者相反，我不在希望收到此消息
//但是当播放完成之后，内容从头尾播放完成，我希望得到些消息，从而进行一步操作，一般来说是重复播放下一遍
bool Player::Stop()
{
    player_->SetCallbackFunc(NULL, (TCallbackMessage)(MsgStop) , mq_);    
    return player_->Stop() == 1;
}
bool Player::IsPlaying()
{
    TStreamStatus st;
    player_->GetStatus(&st);
    return st.fPlay || st.nLoop;
}
