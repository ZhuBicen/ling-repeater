#ifndef PLAYER_SC_HPP
#define PLAYER_SC_HPP

#include "StateChart.hpp"
#include "UiInterface.hpp"
#include "Player.hpp"

class PlayerSc: public IStateChartBase {
public:
    PlayerSc(std::string name, UiInterface* ui, Player& player, LingJson& json);
	~PlayerSc(){};
	bool Init();
	bool terminated(){return false;}
    
private:
    bool waitingForFileStateHandler(Event*);
    void waitingForFileStateEntryAction();
    void waitingForFileStateExitAction();

    bool handlingFileStateHandler(Event*);
    void handlingFileStateEntryAction();
    void handlingFileStateExitAction();

    bool playingStateHandler(Event*);
    void playingStateEntryAction();
    void playingStateExitAction();

    bool normalPlayingStateHandler(Event*);
    void normalPlayingEntryAction();
    void normalPlayingExitAction();

    bool repeatedPlayingStateHandler(Event*);
    void repeatedPlayingEntryAction();
    void repeatedPlayingExitAction();

    bool pausingStateHandler(Event*);
    void pausingStateEntryAction();
    void pausingStateExitAction();

    

private:
    StateT<PlayerSc> waiting_for_file_state_;
    StateT<PlayerSc> handling_file_state_;
    StateT<PlayerSc> playing_state_;
    StateT<PlayerSc> normal_playing_state_;
    StateT<PlayerSc> repeated_playing_state_;
    StateT<PlayerSc> pausing_state_;

private:
    std::wstring file_name_;
    UiInterface* ui_;
    Player&      player_;
    LingJson&    json_;
};


#endif
