#ifndef PROGRESS_INTERFACE_H
#define PROGRESS_INTERFACE_H
#include "Utils.hpp"

const int INVALID_POS = -1;
struct PaintInfo
{
    long current_pos_;
    long start_pos_;
    long end_pos_;
    long length_;
    Sections sections_;
    PaintInfo():  current_pos_(INVALID_POS), start_pos_(INVALID_POS), end_pos_(INVALID_POS), length_(INVALID_POS){}
};

struct PaintUpdateInfo
{
    int  color_;
    long length_;
    long previous_pos_;
    long current_pos_;
};
struct ContextMenuInfo{
    bool mark_;
    int  x_;
    int  y_;
};

class UiInterface
{
public:
    //Manipulate the timer to monitor the player status
    virtual void StartTimer() = 0;
    virtual void StopTimer() = 0;

    virtual void SetFileInfo(const FileInfo& file_info, long file_length) = 0;

    //normal play state action
    //virtual void StartPlay(long pos) = 0;
    //virtual void UpdateNormalPlayingStatus( long pre_pos, long new_pos) = 0;

    virtual void SetStartPos(long pos) = 0;
    virtual void SetEndPos(long pos) = 0;

    //repeated play state action
    //virtual void StartRepeat(long start, long end) = 0; 
    //virtual void UpdateRepeatedPlayingStatus( long start_pos, long end_pos, long pre_pos, long new_pos) = 0;

    virtual void ExitApp() = 0;
    virtual void SetRepo(const char*) = 0;

    virtual void DrawBar(PaintInfo* pi) = 0;
    virtual void UpdateBar(PaintUpdateInfo* pui) = 0;
    virtual void ShowContextMenu(bool mark, int x , int y) = 0;
};
#endif
