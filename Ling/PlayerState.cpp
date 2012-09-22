#include "Precompiled.hpp"
#include "PlayerState.hpp"

///////////////////////////////////////////////////////////////////////////////
// Playing state
///////////////////////////////////////////////////////////////////////////////

PlayingState::PlayingState( my_context ctx ):
    my_base( ctx ){
    LOG(logINFO) << __FUNCTION__;
    //What to do within this state?
    context< PlayerFsm >().ui_->StartTimer();
}
PlayingState::~PlayingState(){
    LOG(logINFO) << __FUNCTION__;
    context< PlayerFsm >().ui_->StopTimer();
}

fsm::result PlayingState::react( const PauseResumeEvent& evt){
    LOG(logINFO) << __FUNCTION__ << " PauseResumeEvent";
    context< PlayerFsm >().player_.Pause();
    //long file_length = context< HandlingFileState >().GetFileLength();
    //context< PlayerFsm >().ui_->UpdateNormalPlayingStatus(0, 0);
    return transit<PausingState>();
    //return fsm::detail::result_utility::make_result( fsm::detail::consumed );    
}


////////////////////////////////////////////////////////////////////////////////
// Normal playing state
////////////////////////////////////////////////////////////////////////////////

NormalPlayingState::NormalPlayingState( my_context ctx ) : my_base( ctx ){
    long cur_pos = context< HandlingFileState >().GetCurrentPos();
    LOG(logINFO) << __FUNCTION__ << " start from: " << cur_pos;
    PaintInfo* pi = new PaintInfo();
    pi->current_pos_ = cur_pos;
    pi->sections_ = context< HandlingFileState >().GetSections();
    pi->length_ = context< PlayerFsm >().player_.GetFileLength();
    context< PlayerFsm >().ui_->DrawBar(pi);
    context< PlayerFsm >().player_.Seek(cur_pos);
    context< PlayerFsm >().player_.Play();
}
NormalPlayingState::~NormalPlayingState()
{
    LOG(logINFO) << __FUNCTION__;
    context< PlayerFsm >().player_.Stop();
} 
fsm::result NormalPlayingState::react( const UpdatePosEvent& evt){

    long pos = context< PlayerFsm >().player_.GetCurrentPos();
    context< HandlingFileState >().SetCurrentPos( pos );
        
    LOG(logDEBUG1) << __FUNCTION__ << evt << " pos = " << pos;
    //Update UI
    PaintUpdateInfo* pui = new PaintUpdateInfo();
    pui->color_ = 0;
    pui->current_pos_  =  context< HandlingFileState >().GetCurrentPos();
    pui->previous_pos_ =  context< HandlingFileState >().GetPreviousPos();
    pui->length_ = context< PlayerFsm >().player_.GetFileLength();
    context< PlayerFsm >().ui_->UpdateBar(pui);
        
    return fsm::detail::result_utility::make_result( fsm::detail::consumed );
    //return fsm::detail::consumed;
}
//设置复读开始点
fsm::result NormalPlayingState::react( const SetStartPosEvent& evt){
    LOG(logINFO) << __FUNCTION__ << evt ;
    long start_pos = context< PlayerFsm >().player_.GetCurrentPos();

        
    context< HandlingFileState>().SetStartPos(start_pos);
        
    return fsm::detail::result_utility::make_result( fsm::detail::consumed );        
}
//设置复读结束点, When end pos is setted, start repeated playing
fsm::result NormalPlayingState::react( const SetEndPosEvent& evt){
    LOG(logINFO) << __FUNCTION__ << evt ;
    long end_pos = context< PlayerFsm >().player_.GetCurrentPos();
    context< HandlingFileState>().SetEndPos( end_pos );
    long start_pos = context< HandlingFileState>().GetStartPos();
    if( end_pos < start_pos){
        context< HandlingFileState>().SetStartPos(0);
    }
    return transit< RepeatedPlayingState >();        
}
fsm::result NormalPlayingState::react( const SetPosEvent& evt){
    LOG(logINFO) << __FUNCTION__;
    PaintInfo* pi = new PaintInfo();
    pi->current_pos_ = evt.pos;
    //Need this field? section?
    pi->sections_ = context< HandlingFileState >().GetSections();
    pi->length_ = context< PlayerFsm >().player_.GetFileLength();    
    context< PlayerFsm >().ui_->DrawBar(pi);
    context< PlayerFsm >().player_.Seek(evt.pos);
    //set the current pos 
    context< HandlingFileState >().SetCurrentPos( evt.pos );
    return fsm::detail::result_utility::make_result( fsm::detail::consumed );        
}

fsm::result NormalPlayingState::react( const PlayFinishedEvent& evt){
    LOG(logINFO) << __FUNCTION__;
    PaintInfo* pi = new PaintInfo();
    pi->current_pos_ = 0;
    pi->sections_ = context< HandlingFileState >().GetSections();
    pi->length_ = context< PlayerFsm >().player_.GetFileLength();    
    context< PlayerFsm >().ui_->DrawBar(pi);
    context< PlayerFsm >().player_.Seek(0);
    context< PlayerFsm >().player_.Play();
    return fsm::detail::result_utility::make_result( fsm::detail::consumed );   
}

fsm::result NormalPlayingState::react( const RequestPaintInfoEvent& evt)
{
    LOG(logINFO) << __FUNCTION__ << "RequestPaintInfoEvent";
    long cur_pos = context< HandlingFileState >().GetCurrentPos();
    //prepare the message 
    PaintInfo* pi = new PaintInfo;
    pi->current_pos_ = cur_pos;
    pi->sections_ = context< HandlingFileState >().GetSections();
    pi->length_ = context< PlayerFsm >().player_.GetFileLength();    
    context< PlayerFsm >().ui_->DrawBar(pi);
    return fsm::detail::result_utility::make_result( fsm::detail::consumed );        
}
////////////////////////////////////////////////////////////////////////////////
// Repeated playing state
////////////////////////////////////////////////////////////////////////////////

RepeatedPlayingState::RepeatedPlayingState( my_context ctx ) : my_base( ctx ){
    LOG(logINFO) << __FUNCTION__ ;
    Repeat();
}

fsm::result RepeatedPlayingState::react( const UpdatePosEvent& evt){

    long pos = context< PlayerFsm >().player_.GetCurrentPos();
    
    context< HandlingFileState >().SetCurrentPos( pos );
    PaintUpdateInfo* pui = new PaintUpdateInfo();
    pui->color_ = 1;
    pui->current_pos_  =  context< HandlingFileState >().GetCurrentPos();
    pui->previous_pos_ =  context< HandlingFileState >().GetPreviousPos();
    context< PlayerFsm >().ui_->UpdateBar(pui);
    LOG(logDEBUG1) << __FUNCTION__ << evt << pos;

    return fsm::detail::result_utility::make_result( fsm::detail::consumed );
}
fsm::result RepeatedPlayingState::react( const ContinuePlayEvent& evt){
    //TODO::
    context< HandlingFileState >().SetStartPos( context< HandlingFileState >().GetEndPos() );
    context< HandlingFileState >().SetCurrentPos( context< HandlingFileState >().GetEndPos() );
    return transit< NormalPlayingState >();
}
    
RepeatedPlayingState::~RepeatedPlayingState(){
    LOG(logINFO) << __FUNCTION__ ;
    long pos = context< PlayerFsm >().player_.Stop();
}
void RepeatedPlayingState::Repeat()
{
    long start_pos = context< HandlingFileState >().GetStartPos();
    long end_pos = context< HandlingFileState >().GetEndPos();
    LOG(logINFO) << __FUNCTION__
              << " start pos = " << start_pos 
              << " end pos = " << end_pos ;
    PaintInfo* pi = new PaintInfo();
    pi->start_pos_ = start_pos;
    pi->end_pos_ = end_pos;
    pi->current_pos_ = start_pos;
    pi->sections_ = context< HandlingFileState >().GetSections();
    pi->length_ = context< PlayerFsm >().player_.GetFileLength();
    context< PlayerFsm >().ui_->DrawBar(pi);
    context< HandlingFileState >().SetCurrentPos( start_pos );
    context< PlayerFsm >().player_.Play(start_pos, end_pos);
}


fsm::result RepeatedPlayingState::react( const PlayFinishedEvent& evt)
{
    //just for loop playing
    LOG(logINFO) << __FUNCTION__ << "PlayFinishedEvent" ;
    Repeat(); 
    return fsm::detail::result_utility::make_result( fsm::detail::consumed ); 
}

fsm::result RepeatedPlayingState::react( const SetPosEvent& evt){
    LOG(logINFO) << __FUNCTION__;

    long start_pos = context< HandlingFileState >().GetStartPos();
    long end_pos = context< HandlingFileState >().GetEndPos();
    long mid_pos = (start_pos + end_pos) / 2;
    if(evt.pos >= mid_pos){
        end_pos = evt.pos;
    }else{
        start_pos = evt.pos;
    }
    context< HandlingFileState >().SetStartPos(start_pos);
    context< HandlingFileState >().SetEndPos(end_pos);
    context< PlayerFsm >().player_.Stop();    
    Repeat();

    return fsm::detail::result_utility::make_result( fsm::detail::consumed );        
}

fsm::result RepeatedPlayingState::react( const SaveSectionEvent& evt)
{
    LOG(logINFO)<< __FUNCTION__ << "SaveSectionEvent";
    context< HandlingFileState >().SaveCurrentSection();
    PaintUpdateInfo* pui = new PaintUpdateInfo();
    pui->color_ = 2;//magic number
    pui->section_.begin_  =  context< HandlingFileState >().GetStartPos();
    pui->section_.end_    =  context< HandlingFileState >().GetEndPos();
    context< PlayerFsm >().ui_->UpdateBar(pui);
    
    return fsm::detail::result_utility::make_result( fsm::detail::consumed );        
}
fsm::result RepeatedPlayingState::react( const RequestPaintInfoEvent& evt)
{
    LOG(logINFO)<< __FUNCTION__ << "RequestPaintInfoEvent";
    long cur_pos = context< HandlingFileState >().GetCurrentPos();
    long start_pos = context< HandlingFileState >().GetStartPos();
    long end_pos = context< HandlingFileState >().GetEndPos();
    PaintInfo* pi = new PaintInfo();
    pi->start_pos_ = start_pos;
    pi->end_pos_ = end_pos;
    pi->current_pos_ = cur_pos;
    pi->sections_ = context< HandlingFileState >().GetSections();
    pi->length_ = context< PlayerFsm >().player_.GetFileLength();
    context< PlayerFsm >().ui_->DrawBar(pi);
    return fsm::detail::result_utility::make_result( fsm::detail::consumed );        
}

fsm::result RepeatedPlayingState::react( const RequestContextMenuInfoEvent& evt)
{
    LOG(logINFO) << __FUNCTION__ << " RequestContextMenuInfoEvent";
    long start_pos = context< HandlingFileState >().GetStartPos();
    long end_pos = context< HandlingFileState >().GetEndPos();
    bool mark = false;
    if ( evt.pos_ >= start_pos && evt.pos_ <= end_pos){
        mark = true;
    }
    context< PlayerFsm >().ui_->ShowContextMenu(mark, evt.x_, evt.y_);
    return fsm::detail::result_utility::make_result( fsm::detail::consumed );

}

fsm::result PausingState::react( const RequestPaintInfoEvent& evt){
    LOG(logINFO) << __FUNCTION__ << "RequestPaintInfoEvent";
    long cur_pos = context< HandlingFileState >().GetCurrentPos();
    //prepare the message 
    PaintInfo* pi = new PaintInfo;
    pi->current_pos_ = cur_pos;
    pi->sections_ = context< HandlingFileState >().GetSections();
    pi->length_ = context< PlayerFsm >().player_.GetFileLength();    
    context< PlayerFsm >().ui_->DrawBar(pi);
    return fsm::detail::result_utility::make_result( fsm::detail::consumed );
}
