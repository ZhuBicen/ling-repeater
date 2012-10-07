#include "Precompiled.hpp"
#include "Player_impl.hpp"
#pragma comment(lib, "libzplay.lib")

int  __stdcall
ZplayCallbackFunc(void* instance, void *user_data, TCallbackMessage message, 
                  unsigned int param1, unsigned int param2)
{
	ZPlay *myplayer = (ZPlay*) instance;

	switch(message)
	{
		case MsgStreamNeedMoreData: // stream needs more data
		{

		}
		return 0;	
	}

	return 0;
}

Player_impl::Player_impl(MessageQueue* mq):mq_(mq){
    player_ = CreateZPlay();
	//no sound card found
	assert(player_->EnumerateWaveOut() != 0);
}
Player_impl::~Player_impl(){
    player_->Release();
}

bool Player_impl::OpenFile(const std::wstring& file_name){
    return player_->OpenFileW(file_name.c_str(), sfAutodetect) != 0;
}

bool Player_impl::CloseFile(){
    return player_->Close() != 0;
}
bool Player_impl::Play(){
    return player_->Play() != 0;
}

bool Player_impl::PlayFromTo(long start_pos, long end_pos)
{
    TStreamTime start_time;
    start_time.ms = start_pos;

    TStreamTime end_time;
    end_time.ms = end_pos;
    
    return player_->PlayLoop(tfMillisecond, &start_time, tfMillisecond, &end_time, 1, 0) != 0;

}

long Player_impl::GetFileLength()
{
    TStreamInfoW info;
    player_->GetStreamInfoW(&info);
    return info.Length.ms;
}
long Player_impl::GetCurrentPos()
{
    TStreamTime pos;
    player_->GetPosition(&pos);
    return pos.ms;
}

bool Player_impl::PlayFrom(long pos)
{
    TStreamTime start_time;
    start_time.ms = pos;

    TStreamTime end_time;
    end_time.ms = GetFileLength();
    
    int ret = player_->PlayLoop(tfMillisecond, &start_time, tfMillisecond, &end_time, 1, 0);
    if( ret == 1){return true;}
    else{
        LOG(logERROR) << "libzplay::PlayLoop failed error info:" << player_->GetError() << std::endl;
        return false;
    }
}
bool Player_impl::Pause()
{
    //TODO::
    return player_->Pause() == 1;
}

bool Player_impl::Resume()
{
    return player_->Resume() == 1;
}
bool Player_impl::Stop()
{
    //TODO::
    return true;
}