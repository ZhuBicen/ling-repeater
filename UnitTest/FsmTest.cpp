#include <boost/test/minimal.hpp>
#include "../Ling/Fsm.h"
#include "UiStub.h"
#include "Player_impl.h"

int test_main( int, char *[] )
{
    UiStub ui;
    PlayerFsm fsm (&ui);
    fsm.process_event(OpenFileEvent(L"file_to_test"));

    int i = 0;
    while(i++ < 10){
        if (i == 2)
        fsm.process_event(SetStartPos());
        fsm.process_event(UpdatePosEvent(i));
    }

    fsm.terminate();
    return 0;
}
