#ifndef UI__STUB__H
#define UI__STUB__H
#include "../Ling/UiInterface.h"

class UiStub: public UiInterface{
    void SetFileName(const std::wstring& file_name){
        LOG(logINFO) << __FUNCTION__ << std::endl;    
    }
    //monitor player progress
    void StartTimer(){
        LOG(logINFO) << __FUNCTION__ << std::endl;
    }
    void StopTimer(){
        LOG(logINFO) << __FUNCTION__ << std::endl;    
    }
    //normal play state action
    void StartPlay(){
        LOG(logINFO) << __FUNCTION__ << std::endl;
    }
    void UpdateNormalPlayingStatus(long length, long pos){
        LOG(logINFO) << __FUNCTION__ << std::endl;
    }
    void StartRepeat() {
        LOG(logINFO) << __FUNCTION__ << std::endl;
    }
    void UpdateRepeatedPlayingStatus(long length, long start, long end, long position){
        LOG(logINFO) << __FUNCTION__ << std::endl;
    }
    void ContinuePlay() {
        LOG(logINFO) << __FUNCTION__ << std::endl;    
    }
    void Repaint(){
        LOG(logINFO) << __FUNCTION__ << std::endl;
    }
};


#endif