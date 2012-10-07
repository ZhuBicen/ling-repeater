#ifndef LING_MESSAGE_QUEUE
#define LING_MESSAGE_QUEUE
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include "StateChart.hpp"

typedef Event Message;

class MessageQueue{

public:
    ~MessageQueue(){
        is_destructed = true;
    }
    MessageQueue():is_destructed(false){}
    //bool IsValid(){return is_destructed == false;}
    bool IsEmpty(){
        boost::mutex::scoped_lock lock(mutex_);
        return messages_.empty();
    }
    Message* GetMessage(){
        if( !IsEmpty()){
            boost::mutex::scoped_lock lock(mutex_);
            Message* msg = messages_.front();
            messages_.pop();
            return msg;
        }else{
            boost::mutex::scoped_lock lock(con_mutex_);
            condition_.wait(con_mutex_);
            boost::mutex::scoped_lock lock2(mutex_);
            Message* msg = messages_.front();
            messages_.pop();
            return msg;
        }
    }
    void PutMessage(Message* msg){
        if( is_destructed ){return;}
        boost::mutex::scoped_lock lock(mutex_);
        messages_.push(msg);
        condition_.notify_one();
    }

private:
    std::queue<Message*> messages_;
    boost::mutex mutex_;
    boost::mutex con_mutex_;
    boost::condition_variable_any condition_;
    bool is_destructed;
};


#endif