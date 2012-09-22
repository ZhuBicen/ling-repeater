#include "Precompiled.hpp"
#include "Playersc.hpp"
PlayerSc::PlayerSc(std::string name, UiInterface* ui, Player& player, LingJson& json):
    IStateChartBase(name), ui_(ui), player_(player), json_(json){}
