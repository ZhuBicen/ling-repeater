#include "Precompiled.hpp"
#include "Playersc.hpp"
#include "log.hpp"
#include "Message.hpp"

#define debug LOG(logDEBUG)
PlayerSc::PlayerSc(std::string name, UiInterface* ui, Player& player, LingJson& json):
    IStateChartBase(name), ui_(ui), player_(player), json_(json),
    waiting_for_file_state_ (this, &PlayerSc::waitingForFileStateHandler, "WaitingForFileState"),
    handling_file_state_(this, &PlayerSc::handlingFileStateHandler, "HandlingFileState"),
    playing_state_(this, &PlayerSc::playingStateHandler, "PlayingState"),
    normal_playing_state_(this, &PlayerSc::normalPlayingStateHandler, "NormalPlayingState"),
    repeated_playing_state_(this, &PlayerSc::repeatedPlayingStateHandler, "RepeatedPlayingState"),
    pausing_state_ (this, &PlayerSc::pausingStateHandler, "PausingState"),
    is_terminated_(false)
{
    waiting_for_file_state_.SetEntryAction(&PlayerSc::waitingForFileStateEntryAction);
    waiting_for_file_state_.SetExitAction(&PlayerSc::waitingForFileStateExitAction);
    handling_file_state_.SetEntryAction(&PlayerSc::handlingFileStateEntryAction);
    handling_file_state_.SetExitAction(&PlayerSc::handlingFileStateExitAction);
    playing_state_.SetEntryAction(&PlayerSc::playingStateEntryAction);
    playing_state_.SetExitAction(&PlayerSc::playingStateExitAction);
    normal_playing_state_.SetEntryAction(&PlayerSc::normalPlayingEntryAction);
    normal_playing_state_.SetExitAction(&PlayerSc::normalPlayingExitAction);
    repeated_playing_state_. SetEntryAction(&PlayerSc::repeatedPlayingEntryAction);
    repeated_playing_state_.SetExitAction(&PlayerSc::repeatedPlayingExitAction);
    pausing_state_.SetEntryAction(&PlayerSc::pausingStateEntryAction);
    pausing_state_.SetExitAction(&PlayerSc::pausingStateExitAction);
        
    playing_state_.SetSuperState(&handling_file_state_);
    pausing_state_.SetSuperState(&handling_file_state_);

    handling_file_state_.SetInitTransition(&playing_state_);
    
    normal_playing_state_.SetSuperState(&playing_state_);
    repeated_playing_state_.SetSuperState(&playing_state_);
    
    playing_state_.SetInitTransition(&normal_playing_state_);

    SetInitialState(&waiting_for_file_state_);

}
bool PlayerSc::Init(){
    ConfInfo* pci = new ConfInfo();
    *pci = json_.GetConfInfo();
    ui_->SetConfInfo(pci);
    return true;
}

// Waiting for file state
bool PlayerSc::waitingForFileStateHandler(Event* evt){
    debug << __FUNCTION__ << *evt;
    switch (evt->Id())
    {
    case EVENT_ID_OPEN_FILE:
        file_name_ = static_cast<OpenFileEvent*>(evt->Data())->file_name;
        StateTransition(&handling_file_state_);
        return true;
    case EVENT_ID_TERMINATE:
        is_terminated_ = true;
        return true;
    case EVENT_ID_REQUEST_PAINT_INFO:
        PaintInfo* pi = new PaintInfo();
        pi->current_pos_ = 0;
        ui_->DrawBar(pi);
        return true;
    }        
    debug << __FUNCTION__ << " discard message" << evt;
    return false;
}

void PlayerSc::waitingForFileStateEntryAction(){
    debug << __FUNCTION__;
}

void PlayerSc::waitingForFileStateExitAction(){
    debug << __FUNCTION__;
}

// Handling file state
bool PlayerSc::handlingFileStateHandler(Event* evt){
    debug << __FUNCTION__ << *evt;
    switch(evt->Id()){
    case EVENT_ID_OPEN_FILE:
    {
        // close current file
        player_.CloseFile();
		OpenFileEvent* ofe = static_cast<OpenFileEvent*>(evt->Data());
        file_name_ = ofe->file_name;
        json_.UpdateFileInfo(file_info_);

        // open the new file
        player_.OpenFile(file_name_);
        file_info_ = json_.GetFileInfo(file_name_);
        ui_->SetFileInfo(file_info_, getFileLength());
        // transit next state
        StateTransition(&normal_playing_state_);
        return true;
    }
    case EVENT_ID_TERMINATE:
        is_terminated_ = true;
        return true;
    case EVENT_ID_REQUEST_CONTEXT_MENU_INFO:
    {
        RequestContextMenuInfoEvent* me = static_cast<RequestContextMenuInfoEvent*>(evt->Data());
        ui_->ShowContextMenu(false, me->x_, me->y_);
        return true;
    }
    default:
        debug << __FUNCTION__ << " discard message" << *evt;
    }

    return false;
}
void PlayerSc::handlingFileStateEntryAction(){
    debug << __FUNCTION__;
    start_pos_ = end_pos_ = current_pos_ = previous_pos_ = 0;
    player_.OpenFile(file_name_);
    // update UI
    file_info_ = json_.GetFileInfo(file_name_);
    ui_->SetFileInfo(file_info_, getFileLength());
}
    
void PlayerSc::handlingFileStateExitAction(){
    debug << __FUNCTION__;
}
/////////////////////////////////////////////////
// Playing State
/////////////////////////////////////////////////
bool PlayerSc::playingStateHandler(Event* evt){
    debug << __FUNCTION__ << *evt;
    switch (evt->Id()){
    case EVENT_ID_PAUSE_RESUME:
        player_.Pause();
        StateTransition(&pausing_state_);
        return true;
    default:
        debug << __FUNCTION__ << " discard message" << *evt;
        return false;
    }
    return false;
}
void PlayerSc::playingStateEntryAction(){
    debug << __FUNCTION__;
    ui_->StartTimer();
}
void PlayerSc::playingStateExitAction(){
    debug << __FUNCTION__;
    ui_->StopTimer();
}

/////////////////////////////////////////////////
// Normal playing state
//////////////////////////////////////////////////
void PlayerSc::normalPlayingEntryAction(){
    debug << __FUNCTION__;
    debug << __FUNCTION__ << " start from: " << current_pos_;
    PaintInfo* pi = new PaintInfo();
    pi->current_pos_ = current_pos_;
    pi->sections_ = file_info_.sections_;
    pi->length_ = player_.GetFileLength();
    ui_->DrawBar(pi);
    player_.Seek(current_pos_);
    player_.Play();
}
bool PlayerSc::normalPlayingStateHandler(Event* evt){
    debug << __FUNCTION__ << *evt;
    switch (evt->Id()){
    case EVENT_ID_UPDATE_POS:
    {
        setCurrentPos(player_.GetCurrentPos());
        // Update UI
        PaintUpdateInfo* pui = new PaintUpdateInfo();
        pui->color_ = 0;
        pui->current_pos_  = current_pos_;
        pui->previous_pos_ =  previous_pos_;
        pui->length_ = player_.GetFileLength();
        ui_->UpdateBar(pui);
        return true;
    }
    case EVENT_ID_SET_START_POS:
        start_pos_ = player_.GetCurrentPos();
        return true;
    case EVENT_ID_SET_END_POS:
        end_pos_ = player_.GetCurrentPos();
        if (end_pos_ < start_pos_) {
            start_pos_ = 0;
        }
        StateTransition(&repeated_playing_state_);
        return true;
    case EVENT_ID_SET_POS:
    {
        SetPosEvent* spe = static_cast<SetPosEvent*>(evt->Data());
        PaintInfo* pi = new PaintInfo();
        
        pi->current_pos_ = spe->pos;
        // Need this field? section?
        pi->sections_ = file_info_.sections_;
        pi->length_ = player_.GetFileLength();    
        ui_->DrawBar(pi);
        player_.Seek(spe->pos);
        // set the current pos 
        current_pos_ = spe->pos;
        return true;
    }
    case EVENT_ID_PLAYING_FINISHED:
    {
        PaintInfo* pi = new PaintInfo();
        pi->current_pos_ = 0;
        pi->sections_ = file_info_.sections_;
        pi->length_ = player_.GetFileLength();    
        ui_->DrawBar(pi);
        player_.Seek(0);
        player_.Play();
        return true;
    }
    case EVENT_ID_REQUEST_PAINT_INFO:
    {
        PaintInfo* pi = new PaintInfo;
        pi->current_pos_ = current_pos_;
        pi->sections_ = file_info_.sections_;
        pi->length_ = player_.GetFileLength();    
        ui_->DrawBar(pi);
        return true;
    }
    default:
        debug << __FUNCTION__ << " discard message" << *evt;
        return false;
    }
    return false;
}

void PlayerSc::normalPlayingExitAction(){
    debug << __FUNCTION__;
    player_.Stop();
}


void PlayerSc::repeatedPlayingEntryAction(){
    debug << __FUNCTION__;
    repeat();
}

bool PlayerSc::repeatedPlayingStateHandler(Event* evt){
    debug << __FUNCTION__ << *evt;
    switch (evt->Id()){
	case EVENT_ID_UPDATE_POS:
    {
        setCurrentPos(player_.GetCurrentPos());
        PaintUpdateInfo* pui = new PaintUpdateInfo();
        pui->color_ = 1;
        pui->current_pos_  =  current_pos_;
        pui->previous_pos_ =  previous_pos_;
        ui_->UpdateBar(pui);
        return true;
    }
    case EVENT_ID_CONTINUE_PLAY:
    {
        start_pos_ = end_pos_;
        setCurrentPos(end_pos_);
        StateTransition(&normal_playing_state_);
        return true;
    }
    case EVENT_ID_PLAYING_FINISHED:
        repeat();
        return true;

    case EVENT_ID_SET_POS:
    {
        SetPosEvent* spe = static_cast<SetPosEvent*>(evt->Data());
        long mid_pos = (start_pos_ + end_pos_) / 2;
        if(spe->pos >= mid_pos){
            end_pos_ = spe->pos;
        }else{
            start_pos_ = spe->pos;
        }

        player_.Stop();    
        repeat();
        return true;
    }
    case EVENT_ID_SAVE_SECTION:
    {
        Section sec;
        sec.begin_ = start_pos_;
        sec.end_ = end_pos_;
        file_info_.sections_.push_back(sec);

        PaintUpdateInfo* pui = new PaintUpdateInfo();
        pui->color_ = 2;//magic number
        pui->section_.begin_  =  start_pos_;
        pui->section_.end_    =  end_pos_;
        ui_->UpdateBar(pui);
        return true;
    }
    case EVENT_ID_REQUEST_PAINT_INFO:
    {
        PaintInfo* pi = new PaintInfo();
        pi->start_pos_ = start_pos_;
        pi->end_pos_ = end_pos_;
        pi->current_pos_ = current_pos_;
        pi->sections_ = file_info_.sections_;
        pi->length_ = player_.GetFileLength();
        ui_->DrawBar(pi);
        return true;
    }

    case EVENT_ID_REQUEST_CONTEXT_MENU_INFO:
    {
        bool mark = false;
        RequestContextMenuInfoEvent* ce = static_cast<RequestContextMenuInfoEvent*> (evt->Data());
        if ( ce->pos_ >= start_pos_ && ce->pos_ <= end_pos_){
            mark = true;
        }
        ui_->ShowContextMenu(mark, ce->x_, ce->y_);
        return true;
    }
    default:
        debug << __FUNCTION__ << " discard message" << *evt;
        return false;
    }
    return false;
}


void PlayerSc::repeatedPlayingExitAction(){
    debug << __FUNCTION__;
	player_.Stop();
}


void PlayerSc::pausingStateEntryAction(){
    debug << __FUNCTION__;

}
bool PlayerSc::pausingStateHandler(Event* evt){
debug << __FUNCTION__ << *evt;
    switch (evt->Id()){
    case EVENT_ID_REQUEST_PAINT_INFO:
    {
        PaintInfo* pi = new PaintInfo;
        pi->current_pos_ = current_pos_;
        pi->sections_ = file_info_.sections_;
        pi->length_ = player_.GetFileLength();    
        ui_->DrawBar(pi);
        return true;
    }
	case EVENT_ID_PAUSE_RESUME:
		StateTransition(playing_state_.DeepHistory());
		return true;
    default:
        debug << __FUNCTION__ << " discard message" << *evt;
        return false;
    }
    return false;
}

void PlayerSc::pausingStateExitAction(){
    debug << __FUNCTION__;
}

void PlayerSc::repeat(){
    LOG(logINFO) << __FUNCTION__
              << " start pos = " << start_pos_ 
              << " end pos = " << end_pos_ ;
    PaintInfo* pi = new PaintInfo();
    pi->start_pos_ = start_pos_;
    pi->end_pos_ = end_pos_;
    pi->current_pos_ = start_pos_;
    pi->sections_ = file_info_.sections_;
    pi->length_ = player_.GetFileLength();
    ui_->DrawBar(pi);
    setCurrentPos( start_pos_ );
    player_.Play(start_pos_, end_pos_);
}
