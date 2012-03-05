#include "Precompiled.hpp"
#include "Fsm.hpp"
#include "PlayerState.hpp"

PlayerFsm::PlayerFsm(UiInterface* ui, Player& player, LingJson& json): 
file_name_(L""), ui_(ui), player_(player), json_(json){
    initiate();
    ui_->SetRepo(json_.GetRepoPath());
}


std::wstring PlayerFsm::GetFileName(){
    return file_name_;
}


bool PlayerFsm::isInPlayingState(){
    return state_downcast<const PlayingState*> () != 0;
}
///////////////////////////////////////////////////////////////////////////////
// Waiting for file state
///////////////////////////////////////////////////////////////////////////////

WaitingForFileState::WaitingForFileState(){
    LOG(logINFO) << __FUNCTION__ ;
}
WaitingForFileState::~WaitingForFileState(){
    LOG(logINFO) << __FUNCTION__ ;
}
fsm::result WaitingForFileState::react( const OpenFileEvent& evt){
        LOG(logINFO) << __FUNCTION__ << evt ;
        return transit< HandlingFileState >(&PlayerFsm::SetFileName, evt);
}

fsm::result WaitingForFileState::react( const TerminateEvent& evt){
    LOG(logINFO) << __FUNCTION__ << " TerminateEvent" ;
    return terminate();
}
fsm::result WaitingForFileState::react( const RequestPaintInfoEvent& evt)
{
    LOG(logINFO) << __FUNCTION__ << " RequestPaintInfoEvent";
    PaintInfo* pi = new PaintInfo();
    pi->current_pos_ = 0;
    context< PlayerFsm >().ui_->DrawBar(pi);
    return fsm::detail::result_utility::make_result( fsm::detail::consumed );        
}

fsm::result WaitingForFileState::react( const RequestContextMenuInfoEvent& evt)
{
    LOG(logINFO) << __FUNCTION__ << " RequestContextMenuInfoEvent";
    context< PlayerFsm >().ui_->ShowContextMenu(false, evt.x_, evt.y_);
    return fsm::detail::result_utility::make_result( fsm::detail::consumed );        
}

///////////////////////////////////////////////////////////////////////////////
// Handling file state
///////////////////////////////////////////////////////////////////////////////

HandlingFileState::HandlingFileState( my_context ctx ): my_base( ctx),
    start_pos_(0), end_pos_(0), current_pos_(0), previous_pos_(0){
    context< PlayerFsm >().player_.OpenFile(
        context< PlayerFsm >().file_name_ );
    //Update UI
    file_info_ = context< PlayerFsm >().json_.GetFileInfo(context< PlayerFsm >().file_name_);
    context< PlayerFsm >().ui_->SetFileInfo( file_info_, GetFileLength());
}
long HandlingFileState::GetFileLength(){
    return context< PlayerFsm >().player_.GetFileLength();
}

fsm::result HandlingFileState::react( const OpenFileEvent& evt){
    LOG(logINFO) << __FUNCTION__ << evt ;
    context< PlayerFsm >().player_.CloseFile();
    context< PlayerFsm >().file_name_ = evt.file_name;
    context< PlayerFsm >().json_.UpdateFileInfo(file_info_);    
    //close current file
    context< PlayerFsm >().player_.OpenFile(
            context< PlayerFsm >().file_name_);
    //open the new file
    file_info_ = context< PlayerFsm >().json_.GetFileInfo(context< PlayerFsm >().file_name_);
    context< PlayerFsm >().ui_->SetFileInfo( file_info_, GetFileLength());

    return transit< NormalPlayingState >();
}

fsm::result HandlingFileState::react( const TerminateEvent& evt){
    LOG(logINFO) << __FUNCTION__ << "TerminateEvent" ;
    return terminate();
}

//Warn:: same as WaitingForFileState::RequestContextMenuInfoEvent react
fsm::result HandlingFileState::react( const RequestContextMenuInfoEvent& evt)
{
    LOG(logINFO) << __FUNCTION__ << " RequestContextMenuInfoEvent";
    context< PlayerFsm >().ui_->ShowContextMenu(false, evt.x_, evt.y_);
    return fsm::detail::result_utility::make_result( fsm::detail::consumed );
}

