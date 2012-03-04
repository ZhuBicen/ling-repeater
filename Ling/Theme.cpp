#include "Precompiled.hpp"
#include "Theme.hpp"

void Theme::Init(){
    index_ = 0;
    themes_[2] = new Native();
    themes_[1] = new QQ();
    themes_[0] = new DeepBlue();
}
Theme::ITheme* Theme::Get(){
    return themes_[index_];
}

void Theme::Change()
{
    index_ = (index_ + 1) % 3;
}

