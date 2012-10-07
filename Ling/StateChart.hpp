#ifndef STATE_CHART_HPP
#define STATE_CHART_HPP
#include <string>
#include <vector>
class Event {
public:
    Event(int id, void* data, int length): id_(id), data_(data), length_(length){}
	int Id()const { return id_;}
    void* Data(){ return data_;}
    int Length(){ return length_;}
private:
    int id_;
    void* data_;
    int length_;
};


class IStateBase {
public:
    IStateBase(std::string const & name): name_(name), super_state_(0),
                                          deep_history_(0), shallow_history_(0){
    }
    virtual ~IStateBase(){};
    
    std::string const& Name() const { return name_;}

    void SetSuperState(IStateBase* super_state) {
        super_state_ = super_state;
    }

    IStateBase* SuperState() const { return super_state_; }

    void SetShallowHistory(IStateBase* shallow_history) {
        shallow_history_ = shallow_history;
    }

    IStateBase* ShallowHistory() {
        if(shallow_history_)
        {
            return shallow_history_;
        }
        return this;
    }
    void SetDeepHistory(IStateBase* deep_history) {
        deep_history_ = deep_history;
    }
    
    IStateBase* DeepHistory() {
        if(deep_history_) {
            return deep_history_;
        }
        return this;
    }
    
    virtual bool HandleEvent(Event* event) = 0;
    
    virtual void InitTransition() = 0;
    
    virtual void EntryAction() = 0;

    virtual void ExitAction() = 0;

private:
    std::string name_;
    IStateBase*  super_state_;
    IStateBase*  deep_history_;
    IStateBase*  shallow_history_;
};
    

template <typename STATE_CHART>
class StateT : public IStateBase
{

public:	

    StateT(STATE_CHART* psc, bool(STATE_CHART::*state_handler)(Event* event), 
            std::string const & name): IStateBase(name) { 
        psc_ = psc; 
        state_handler_ = state_handler;
        entry_action_ = 0;
        exit_action_ = 0;
        init_state_ = 0;
    };


    virtual ~StateT(){};

    virtual bool HandleEvent(Event* event) { 
        return (*psc_.*state_handler_)(event);
    }

    void SetInitTransition(IStateBase* init_state) {
        init_state_ = init_state;
    }


    void SetEntryAction(void (STATE_CHART::*entry_action)()){
        entry_action_ = entry_action; 
    }

    void SetExitAction(void (STATE_CHART::*exit_action)()){
        exit_action_ = exit_action;
    }

    void InitTransition()  {
        if(init_state_) { 
            psc_->StateTransition(init_state_);
        }
    }

    virtual void EntryAction() {
        if(entry_action_){
            (*psc_.*entry_action_)();
        }
    }

    virtual void ExitAction() {
        if(exit_action_) {
            (*psc_.*exit_action_)();
        }
    }
    
private:
    STATE_CHART* psc_;
    bool (STATE_CHART::*state_handler_)(Event* event);
    IStateBase*  init_state_;

    void (STATE_CHART::*entry_action_)();
    void (STATE_CHART::*exit_action_)();

};

class IStateChartBase
{
public:

    IStateChartBase(const std::string& name):
        current_state_(0), next_state_(0){}

    virtual ~IStateChartBase(){}

    std::string Name(){return name_;}

    virtual bool Init() = 0;
    void SetInitialState(IStateBase* initial_state) {
        StateTransition(initial_state);
        EnterNextState();
    }

    bool ProcessEvent(Event* event){
        if (current_state_ == 0) {
            return false;
        }

        // ConsumeEvent may change the next_state_
        if (!ConsumeEvent(event)) {
            return false;
        }

        if (next_state_) {
            EnterNextState();
        }
        return true;
    }
    
    void StateTransition(IStateBase* next_state) {
        next_state_ = next_state;
    }

    IStateBase const * const CurrentState() const {
        return current_state_;
    }

private:

    bool ConsumeEvent(Event* event) {
        IStateBase* state = current_state_;

        bool consumed = state->HandleEvent(event);

        while (consumed == false && state->SuperState()) {
			state = state->SuperState();
            consumed = state->HandleEvent(event);
        }

        return consumed;
    }
        

    void CallExitActionsAndSetHistoryStates(IStateBase* current_state, IStateBase* next_state) {
        // only initial state is setting
        if (current_state == 0 ){
            return;
        }

        // self transition
        if (current_state == next_state) {
            current_state->ExitAction();
            return;
        }

        IStateBase* exiting_state = current_state;
		IStateBase* target_state;

        bool end_flag = false;
		do {
			end_flag = false;
			target_state = next_state;
            while( target_state != 0 && end_flag == false){
                if (exiting_state == target_state->SuperState()){
                    end_flag = true;
                    exiting_state = 0;
                }else{
                    target_state = target_state->SuperState();
                }
            }
            if (end_flag == false){
                if (exiting_state->SuperState() != 0){
                    exiting_state->SuperState()->SetShallowHistory(exiting_state);
                    exiting_state->SuperState()->SetDeepHistory(current_state);
                }
                exiting_state->ExitAction();
                exiting_state = exiting_state->SuperState();
            }
        }while( exiting_state != 0);

    }

    void CallEntryActions(IStateBase* current_state, IStateBase* next_state){
        IStateBase* state = next_state;
        std::vector<IStateBase*> enter_states;

        // entry acton for the next state will be executed anyway
        enter_states.push_back(next_state);

        while (state = state->SuperState()) {

            // check entry action of 'state' need to execute
            IStateBase* source_state = current_state;
            while( source_state != 0 ){
                if (state == source_state) {
                    goto exitloop;
                }
                source_state = source_state->SuperState();
            }
            // the entry action should be executed
            enter_states.push_back(state);
        }

        
    exitloop:
        for (int index = enter_states.size() -1; index >= 0; index--){
            enter_states[index]->EntryAction();
        }
    }
                    
                
        

    void EnterNextState() {
        while (next_state_ != 0) {
            
            CallExitActionsAndSetHistoryStates(current_state_, next_state_);
            CallEntryActions(current_state_, next_state_);
            
            current_state_ = next_state_;
            next_state_ = 0;
            
            current_state_->InitTransition();
		}
	}


    std::string name_;

    IStateBase* current_state_;

    IStateBase* next_state_;

};

#endif

