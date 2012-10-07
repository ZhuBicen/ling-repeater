#include <boost/test/unit_test.hpp>
#include <boost/thread.hpp>
#include <boost/ref.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "../ling/messagequeue.hpp"
#include "../ling/message.hpp"
#include <iostream>
void sendThread(MessageQueue& mq)
{
    mq.PutMessage(boost::shared_ptr<Message>(new OpenFileEvent(L"Test.mp3")));
    boost::this_thread::sleep(boost::posix_time::seconds(2));
    mq.PutMessage(boost::shared_ptr<Message>(new OpenFileEvent(L"Test2.mp3")));
}

void getThread(MessageQueue& mq)
{
    {
    boost::shared_ptr<Message> msg = mq.GetMessage();
    OpenFileEvent* ofe = dynamic_cast<OpenFileEvent*>(msg.get());
    BOOST_CHECK(ofe->file_name == L"Test.mp3");
    }
    {
    boost::shared_ptr<Message> msg = mq.GetMessage();
    OpenFileEvent* ofe = dynamic_cast<OpenFileEvent*>(msg.get());
    BOOST_CHECK(ofe->file_name == L"Test2.mp3");
    }
}

BOOST_AUTO_TEST_CASE( MessageQueueTest )
{
    MessageQueue mq;
    boost::thread st(sendThread, boost::ref(mq));
    boost::thread gt(getThread,  boost::ref(mq));
    st.join();
    gt.join();
}