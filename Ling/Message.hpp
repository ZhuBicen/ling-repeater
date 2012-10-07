#ifndef LING_MESSAGE_HPP
#define LING_MESSAGE_HPP
#include <iostream>
#include <string>
#include <boost/statechart/event.hpp> 
namespace fsm = boost::statechart;

//打开文件
struct OpenFileEvent: public fsm::event< OpenFileEvent >{
    const std::wstring file_name;
    explicit OpenFileEvent(const std::wstring p_file_name):file_name(p_file_name){}
};

std::ostream& operator<< (std::ostream& os, const OpenFileEvent& evt);

//更新播放进度
struct UpdatePosEvent: public fsm::event< UpdatePosEvent >{
};
std::ostream& operator<< (std::ostream& os, const UpdatePosEvent& evt);

//设置开始点
struct SetStartPosEvent: public fsm::event < SetStartPosEvent >{
};

std::ostream& operator<< (std::ostream& os, const SetStartPosEvent& evt);

//设置结束点
struct SetEndPosEvent: public fsm::event < SetEndPosEvent >{
};

std::ostream& operator<< (std::ostream& os, const SetEndPosEvent& evt);

//文件播放完毕
struct StopFileEvent: public fsm::event< StopFileEvent>{};

//继续播放
struct ContinuePlayEvent: public fsm::event< ContinuePlayEvent >{};

struct StartRepeatEvent: public fsm::event< StartRepeatEvent > {};

struct SetPosEvent: public fsm::event< SetPosEvent >{
    long pos;
    explicit SetPosEvent(long p):pos(p){}
};
//暂停播放
struct PauseResumeEvent: public fsm::event< PauseResumeEvent>{};
//terminate the fsm
struct TerminateEvent: public fsm::event <TerminateEvent>{};

struct PlayFinishedEvent: public fsm::event< PlayFinishedEvent>{};
struct SaveSectionEvent: public fsm::event< SaveSectionEvent>{};

struct RequestPaintInfoEvent: public fsm::event<RequestPaintInfoEvent>{};

struct RequestContextMenuInfoEvent: public fsm::event<RequestContextMenuInfoEvent>{
    long pos_;
    int x_;
    int y_;
    RequestContextMenuInfoEvent(long pos, long x, long y): pos_(pos), x_(x), y_(y){}
};
#endif