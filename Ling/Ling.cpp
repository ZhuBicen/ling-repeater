// Ling.cpp : main source file for Ling.exe
//

#include "Precompiled.hpp"
#include "MainWindow.hpp"
#include "MainWindow.hpp"
#include "MessageQueue.hpp"
#include "Utils.hpp"
#include "Playersc.hpp"
#include "Theme.hpp"

#pragma comment(lib, "gdiplus.lib")

CAppModule _Module;

void FsmThreadFunc(MessageQueue& mq, UiInterface* ui, LingJson& json)
{
    Player player(&mq);
    PlayerSc fsm("PlayerFsm", ui, player, json);

    for(;;){
        if( !fsm.terminated()){
            boost::shared_ptr<Message> msg = mq.GetMessage();
            fsm.ProcessEvent(msg.get());
            //boost::this_thread::sleep(boost::posix_time::seconds(2));
        }
        else{
            //when the user click the close button, one TerminateEvent will be processed by the fsm, 
            //then the fsm  teminated it's self, then here post one message to the GUI to exit.
            ui->ExitApp();
            LOG(logINFO) << "Exit the fsm thread" ;
            return;
        }
    }
}

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
    CMessageLoop theLoop;
    _Module.AddMessageLoop(&theLoop);

    //Create the Message Queue
    MessageQueue message_queue;
    LingJson json;
    Theme::Init();
    LOG(logINFO) << json;
    CMainDlg wndMain(message_queue);
	if(wndMain.Create(NULL) == NULL)
	{
		ATLTRACE(_T("Main dialog creation failed!\n"));
		return 0;
	}

    wndMain.ShowWindow(nCmdShow);


    //Create the fsm thread
    boost::thread fsm_thread(FsmThreadFunc, boost::ref(message_queue), &wndMain, boost::ref(json));
    fsm_thread.detach();

    int nRet = theLoop.Run();

    _Module.RemoveMessageLoop();
    return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
    if( AttachConsole(ATTACH_PARENT_PROCESS)){
        freopen("CONOUT$", "w+t", stdout);
        freopen("CONOUT$", "w+t", stderr);

    }
    else{
#if defined(DEBUG)
        AllocConsole();
        freopen("CONOUT$", "w+t", stdout);
        freopen("CONOUT$", "w+t", stderr);
#endif
    }
    LOG(logINFO)<<"WinMain is started";


    
    HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//  HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
    ATLASSERT(SUCCEEDED(hRes));
    //Startup Gdi plus
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken; 
    Gdiplus::Status st = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
        
    // this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
    ::DefWindowProc(NULL, 0, 0, 0L);

    AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

    hRes = _Module.Init(NULL, hInstance);
    ATLASSERT(SUCCEEDED(hRes));

    int nRet = Run(lpstrCmdLine, nCmdShow);

    _Module.Term();
    ::CoUninitialize();

    GdiplusShutdown(gdiplusToken);
    return nRet;
}
