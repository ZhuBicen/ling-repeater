#include "Precompiled.hpp"
#include "Playersc.hpp"
#include "log.hpp"

#define debug LOG(logDEBUG)
PlayerSc::PlayerSc(std::string name, UiInterface* ui, Player& player, LingJson& json):
    IStateChartBase(name), ui_(ui), player_(player), json_(json),
    waiting_for_file_state_ (this, &PlayerSc::waitingForFileStateHandler, "WaitingForFileState"),
    handling_file_state_(this, &PlayerSc::handlingFileStateHandler, "HandlingFileState"),
    playing_state_(this, &PlayerSc::playingStateHandler, "PlayingState"),
    normal_playing_state_(this, &PlayerSc::normalPlayingStateHandler, "NormalPlayingState"),
    repeated_playing_state_(this, &PlayerSc::repeatedPlayingStateHandler, "RepeatedPlayingState"),
    pausing_state_ (this, &PlayerSc::pausingStateHandler, "PausingState")
    {
        waiting_for_file_state_.SetEntryAction(&PlayerSc::waitingForFileStateEntryAction);
        waiting_for_file_state_.SetExitAction(&PlayerSc::waitingForFileStateExitAction);
        handling_file_state_.SetEntryAction(&PlayerSc::handlingFileStateEntryAction);
        handling_file_state_.SetExitAction(&PlayerSc::playingStateExitAction);
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
    return true;
}

bool PlayerSc::waitingForFileStateHandler(Event*){
    return true;
}

void PlayerSc::waitingForFileStateEntryAction(){
    debug << __FUNCTION__;
}

void PlayerSc::waitingForFileStateExitAction(){
    debug << __FUNCTION__;
}

bool PlayerSc::handlingFileStateHandler(Event*){
    return true;
}
void PlayerSc::handlingFileStateEntryAction(){
    debug << __FUNCTION__;
}
    
void PlayerSc::handlingFileStateExitAction(){
    debug << __FUNCTION__;
}

bool PlayerSc::playingStateHandler(Event*){
    return true;
}
void PlayerSc::playingStateEntryAction(){
    debug << __FUNCTION__;
}
void PlayerSc::playingStateExitAction(){
    debug << __FUNCTION__;
}

bool PlayerSc::normalPlayingStateHandler(Event*){
    return true;
}
void PlayerSc::normalPlayingEntryAction(){
    debug << __FUNCTION__;
}
void PlayerSc::normalPlayingExitAction(){
    debug << __FUNCTION__;
}

bool PlayerSc::repeatedPlayingStateHandler(Event*){
    return true;
}
void PlayerSc::repeatedPlayingEntryAction(){
    debug << __FUNCTION__;
}
void PlayerSc::repeatedPlayingExitAction(){
    debug << __FUNCTION__;
}

bool PlayerSc::pausingStateHandler(Event*){
    return true;
}
void PlayerSc::pausingStateEntryAction(){
    debug << __FUNCTION__;
}
void PlayerSc::pausingStateExitAction(){
    debug << __FUNCTION__;
}
