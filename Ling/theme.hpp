#ifndef __THEME__HPP
#define __THEME__HPP
#include "Precompiled.hpp"
namespace Theme{
const Color Black = Color(32 , 33 , 34 );
const Color Pink  = Color(218, 42 , 151);
const Color Blue  = Color(24 , 141, 211);
const Color White = Color(255, 255, 255);
const Color Gray  = Color(68 , 68 , 68 );

const Color Yellow = Color(214, 112, 0);
const Color Red = Color(188, 31, 31);

class ITheme{
public:
    virtual Color PlayColor() = 0;
    virtual Color RepeateColor() = 0;
    virtual Color SectionColor() = 0;
    virtual Color BgColor() = 0;
    virtual Color BarBgColor() = 0;
    virtual Color BarFrame1Color() = 0;
    virtual Color BarFrame2Color() = 0;
};

class Native: public ITheme{
public:
    Color PlayColor() {return Blue;}
    Color RepeateColor() {return White;}
    Color SectionColor() { return Pink;}
    Color BgColor() { return Black;}
    Color BarBgColor() {return Gray;}
    Color BarFrame1Color() { return Color(25, 25, 25);}
    Color BarFrame2Color(){ return Color(66, 66, 66);}
};

class QQ:public ITheme{
public:
    Color PlayColor(){return Red;}
    Color RepeateColor(){return Yellow;}
    Color SectionColor(){return Blue;}
    Color BgColor(){return Color(55, 186, 235);}
    Color BarBgColor(){return Gray;}
    Color BarFrame1Color() { return Color(25, 25, 25);}
    Color BarFrame2Color(){ return Color(66, 66, 66);}
};

class DeepBlue:public ITheme{
public:
    Color PlayColor() {return Blue;}
    Color RepeateColor() {return White;}
    Color SectionColor() { return Pink;}
    Color BgColor() { return Color(72, 101, 154);}
    Color BarBgColor() {return Color(72, 96, 118);}
    Color BarFrame1Color() { return Color(25, 25, 25);}
    Color BarFrame2Color(){ return Color(66, 66, 66);}
};


void Init();
ITheme* Get();
void Change();
static ITheme* themes_[3];
static int index_;

class Redrawer{
public:
    virtual void Redraw() = 0;
};
}
#endif