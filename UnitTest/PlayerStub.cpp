#include <cassert>
#include <iostream>

#include "Player_impl.h"

Player_impl::Player_impl()
{
}

Player_impl::~Player_impl()
{
}
void Player_impl::Init(HWND hwnd, HINSTANCE hInst)
{
}
bool Player_impl::OpenFile(const std::wstring& file_name)
{
    return true;
}
bool Player_impl::CloseFile()
{
    return true;
}
bool Player_impl::PlayFile()
{
    state_.length = 100;
    state_.current_state = PlayerState::play;
    //state_.file_name = file_name;
    return true;
}
std::wstring Player_impl::GetFileName()
{
    return state_.file_name;
}
long Player_impl::GetFileLength()
{
    return state_.length;
}
bool Player_impl::Repeat()
{
    state_.current_state = PlayerState::repeate;
    //state_.end_pos = MCIWndGetPosition(playerWnd_);
    return true;//MCIWndPlayFromTo(playerWnd_, state_.start_pos, state_.end_pos) == 0;
}
const PlayerState& Player_impl::GetState()
{
    state_.curent_pos = 0;//MCIWndGetPosition(playerWnd_);
    return state_;
}

bool Player_impl::SetStartPos()
{
    state_.start_pos = 0;//MCIWndGetPosition(playerWnd_);
    std::cout << "Setting started position: " << state_.start_pos << std::endl;
    return true;
}
bool Player_impl::PlayFrom(long pos)
{
    if(state_.current_state == PlayerState::repeate){
        assert(false);
    }
    if(state_.current_state == PlayerState::play){
        state_.end_pos = pos;
    }
	return true;
}
bool Player_impl::ContinuePlay()
{
    state_.current_state = PlayerState::play;
    return true;
}
bool Player_impl::Pause()
{
	return true;
}
bool Player_impl::Stop()
{
    return true;
}