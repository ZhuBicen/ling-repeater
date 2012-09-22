#ifndef LING_STATE_HPP_INCLUDED
#define LING_STATE_HPP_INCLUDED

#include "Fsm.hpp"
#include <boost/statechart/event.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/list.hpp>

namespace fsm = boost::statechart;
///////////////////////////////////////////////////////////////////////////////
// Playing state
///////////////////////////////////////////////////////////////////////////////
class PlayingState:
public fsm::state< PlayingState, HandlingFileState, NormalPlayingState,  fsm::has_deep_history >
{
public:
    PlayingState( my_context ctx );
    ~PlayingState();
    typedef fsm::custom_reaction< PauseResumeEvent > reactions;
    
    //文件播放完毕
    fsm::result react( const PauseResumeEvent& evt);


};
////////////////////////////////////////////////////////////////////////////////
// Normal playing state
////////////////////////////////////////////////////////////////////////////////
class NormalPlayingState: public fsm::state< NormalPlayingState, PlayingState >
{
public:
    NormalPlayingState( my_context ctx );
    ~NormalPlayingState();
    
    typedef boost::mpl::list<
        fsm::custom_reaction< UpdatePosEvent >,
        fsm::custom_reaction< SetStartPosEvent>,        
        fsm::custom_reaction< SetEndPosEvent >,
        fsm::custom_reaction< SetPosEvent >, 
        fsm::custom_reaction< PlayFinishedEvent>,
        fsm::custom_reaction< RequestPaintInfoEvent>
        > reactions;
    
    fsm::result react( const UpdatePosEvent& evt);
    //设置复读开始点
    fsm::result react( const SetStartPosEvent& evt);
    
    //设置复读结束点, When end pos is setted, start repeated playing
    fsm::result react( const SetEndPosEvent& evt);
    fsm::result react( const SetPosEvent& evt);

    fsm::result react( const PlayFinishedEvent& evt);
    fsm::result react( const RequestPaintInfoEvent& evt);
};
////////////////////////////////////////////////////////////////////////////////
// Repeated playing state
////////////////////////////////////////////////////////////////////////////////
class RepeatedPlayingState: public fsm::state< RepeatedPlayingState, PlayingState >
{
public:
    RepeatedPlayingState( my_context ctx );
    typedef boost::mpl::list<
        fsm::custom_reaction< UpdatePosEvent >,
        fsm::custom_reaction< ContinuePlayEvent >,
        fsm::custom_reaction< SetPosEvent >, 
        fsm::custom_reaction< SaveSectionEvent>,
        fsm::custom_reaction< PlayFinishedEvent>,
        fsm::custom_reaction< RequestContextMenuInfoEvent>,
        fsm::custom_reaction< RequestPaintInfoEvent>
        > reactions;

    fsm::result react( const UpdatePosEvent& evt);
    fsm::result react( const ContinuePlayEvent& evt);
    fsm::result react( const SetPosEvent& evt);
    fsm::result react( const PlayFinishedEvent& evt);
    fsm::result react( const SaveSectionEvent&);
    fsm::result react( const RequestPaintInfoEvent& evt);
    fsm::result react( const RequestContextMenuInfoEvent& evt);
    
    ~RepeatedPlayingState();
    void Repeat();
};

////////////////////////////////////////////////////////////////////////////////
// Repeated playing state
////////////////////////////////////////////////////////////////////////////////
class PausingState: public fsm::simple_state< PausingState, HandlingFileState >
{
public:
    typedef boost::mpl::list<
        fsm::custom_reaction< PauseResumeEvent >, 
        fsm::custom_reaction< RequestPaintInfoEvent >
        > reactions; 

    fsm::result react( const PauseResumeEvent& evt){
         return transit<fsm::deep_history<NormalPlayingState > >();
    }
    fsm::result react( const RequestPaintInfoEvent& evt);
};
#endif
