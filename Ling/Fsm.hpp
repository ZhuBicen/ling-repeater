#ifndef PLAYER__FSM__H
#define PLAYER__FSM__H
#include "PreCompiled.hpp"
#include "UiInterface.hpp"
#include "Player.hpp"
#include "Message.hpp"
#include "log.hpp"
#include "Utils.hpp"
    
class WaitingForFileState;
class NormalPlayingState;
class StoppedState;

class PlayerFsm : public fsm::state_machine< PlayerFsm,  WaitingForFileState >
{
public:
    friend class WaitingForFileState;
    friend class HandlingFileState;
    friend class PlayingState;
    friend class NormalPlayingState;
    friend class RepeatedPlayingState;
    friend class TerminatedState;
    friend class PausingState;

    PlayerFsm(UiInterface* ui, Player& player, LingJson& json);
    ~PlayerFsm(){}
    void SetFileName(const OpenFileEvent& evt){
        file_name_ = evt.file_name;
    }
    std::wstring GetFileName();

    bool isInPlayingState();
private:
    std::wstring file_name_;
    UiInterface* ui_;
    Player& player_;
    LingJson& json_;
};

class WaitingForFileState: public fsm::simple_state< WaitingForFileState, PlayerFsm >
{
public:
    WaitingForFileState();
    ~WaitingForFileState();
    typedef boost::mpl::list<
        fsm::custom_reaction< OpenFileEvent >,
        fsm::custom_reaction< RequestPaintInfoEvent>,
        fsm::custom_reaction< RequestContextMenuInfoEvent>,
        fsm::custom_reaction< TerminateEvent >
    > reactions;
    // Shit, this can NOT work.
    //typedef fsm::transition< OpenFileEvent, HandlingFileState, PlayerFsm, &PlayerFsm::SetFileName> reactions;
    fsm::result react( const OpenFileEvent& evt);
    fsm::result react( const TerminateEvent& evt);    
    fsm::result react( const RequestPaintInfoEvent& evt);
    fsm::result react( const RequestContextMenuInfoEvent& evt);
};

class HandlingFileState: public fsm::state< HandlingFileState, PlayerFsm, PlayingState >
{
public:
    HandlingFileState( my_context ctx );
    ~HandlingFileState(){
        context< PlayerFsm >().json_.UpdateFileInfo(file_info_);
    }

    long GetFileLength();
    typedef boost::mpl::list<
        fsm::custom_reaction< OpenFileEvent >,
        fsm::custom_reaction< RequestContextMenuInfoEvent>,
        fsm::custom_reaction< TerminateEvent >
    > reactions;

    fsm::result react( const OpenFileEvent& evt);
    fsm::result react( const TerminateEvent& evt);
    fsm::result react( const RequestContextMenuInfoEvent& evt);


    void SetStartPos(long start_pos){
        LOG(logINFO) << " setting start pos = " << start_pos;
        start_pos_ = start_pos;
        context< PlayerFsm >().ui_->SetStartPos(start_pos);        
    }
    long GetStartPos(){
        return start_pos_;
    }
    void SetEndPos(long end_pos){
        LOG(logINFO) << " setting end pos = " << end_pos;
        end_pos_ = end_pos;
        context< PlayerFsm >().ui_->SetEndPos(end_pos);        
    }
    long GetEndPos(){
        return end_pos_;
    }

    void SetCurrentPos(long current_pos){
        previous_pos_ = current_pos_;
        current_pos_ = current_pos;
    }
    long GetCurrentPos(){
        return current_pos_;
    }
    long GetPreviousPos(){
        return previous_pos_;
    }
    void SaveCurrentSection(){
        Section sec;
        sec.begin_ = start_pos_;
        sec.end_ = end_pos_;
        file_info_.sections_.push_back(sec);
    }

    Sections GetSections(){return file_info_.sections_;}
private:
    long start_pos_;
    long end_pos_;
    long current_pos_;
    long previous_pos_;
    
    FileInfo file_info_;
};


//class TerminatedState:public fsm::state<TerminatedState , PlayerFsm>
//{
//public:
//    TerminatedState( my_context ctx ):my_base( ctx)
//    {
//        context< PlayerFsm >().ui_->ExitApp();
//        //terminate();
//    }
//};
#endif
