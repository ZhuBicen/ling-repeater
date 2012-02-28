#include "Precompiled.hpp"
#include "MainWindow.hpp"
#include "CGdiPlusBitmap.h"
#include "Utils.hpp"
#include "AboutDlg.h"
extern CAppModule _Module;

const UINT MainWindow::TASKBAR_CREATE_MESSAGE = RegisterWindowMessage ( _T("TaskbarButtonCreated") );

MainWindow::MainWindow(MessageQueue& mq): rect_(0, 0, 550, 50),
                          title_rect_(0, 0, 550, 20),
                          icon_rect_(3, 3, 20, 20),
                          file_name_rect_(60, 7, 400, 20),
                          bar_rect_(3, 27, 546, 44), 
                          close_rect_(525, 2, 543, 20),
                          bar_(*this),
                          mq_(mq),
                          close_button_(L"CLOSE_NORMAL", L"CLOSE_HOVER"),
                          pin_rect_(504, 2, 523, 20),
                          a_rect_(484, 2, 502, 20),
                          a_button_(L"A_NORMAL", L"A_NORMAL"),
                          b_button_(L"B_NORMAL", L"B_NORMAL"),
                          pin_button_(L"PIN_HOVER", L"PIN_NORMAL")
{
    Create(NULL, rect_, TEXT("风语"));
    LOG(logINFO) << __FUNCTION__ << " thread id =  " << GetCurrentThreadId();
    LOG(logINFO) << __FUNCTION__ << " HWND = 0x" << std::hex << this->m_hWnd << std::dec ;

    HRGN hRgn = CreateRoundRectRgn(0, 0, rect_.Width(), rect_.Height(), 3, 3);
    SetWindowRgn(hRgn,FALSE);

    int width = ::GetSystemMetrics(SM_CXFULLSCREEN);
    SetWindowPos(HWND_TOPMOST, width / 4, 500, 0, 0, SWP_NOSIZE);

    close_button_.Create(m_hWnd, close_rect_, NULL, WS_CHILD|BS_OWNERDRAW, 0, IDB_CLOSE);
    close_button_.ShowWindow(SW_SHOW);

    //a_button_.Create(m_hWnd, a_rect_, NULL, WS_CHILD|BS_OWNERDRAW, 0, IDB_A);
    //a_button_.ShowWindow(SW_SHOW);

    //b_button_.Create(m_hWnd, b_rect_, NULL, WS_CHILD|BS_OWNERDRAW, 0, IDB_B);
    //b_button_.ShowWindow(SW_SHOW);

    pin_button_.Create(m_hWnd, pin_rect_, NULL, WS_CHILD|BS_OWNERDRAW, 0, IDB_PIN);
    pin_button_.ShowWindow(SW_SHOW);

    file_name_static_.Create(m_hWnd, file_name_rect_, TEXT("TEST"), WS_CHILD | SS_BLACKFRAME |SS_LEFT);
    file_name_static_.ShowWindow(SW_SHOW);

    bar_.Create(*this, bar_rect_, TEXT("PROGRESS_BAR"));
    DragAcceptFiles(TRUE);

    hotkey_ = GlobalAddAtom(TEXT("ZHUBICEN_REPEATER"));
    if (!RegisterHotKey(m_hWnd, hotkey_, 0, VK_F1)){
        LOG(logERROR) << "Can't register the hot key F1" ;
    }
    if (!RegisterHotKey(m_hWnd, hotkey_ + 1, 0, VK_F2)){
        LOG(logERROR) << "Can't register the hot key F2" ;
    }
    if (!RegisterHotKey(m_hWnd, hotkey_ + 2, 0, VK_F3)){
        LOG(logERROR) << "Can't register the hot key F3" ;
    }
    if (!RegisterHotKey(m_hWnd, hotkey_ + 3, 0, VK_F4)){
        LOG(logERROR) << "Can't register the hot key F4" ;
    }
    if (!RegisterHotKey(m_hWnd, hotkey_ + 4, 0, VK_F5)){
        LOG(logERROR) << "Can't register the hot key F5" ;
    }

    //CMessageLoop* pLoop = _Module.GetMessageLoop();
    //ATLASSERT(pLoop != NULL);
    //pLoop->AddMessageFilter(this);
}
void MainWindow::OnHotKey(int nHotKeyID, UINT uModifiers, UINT uVirtKey)
{
    LOG(logINFO) << __FUNCTION__ ;
    //F2 设置开始点
    if(uVirtKey == VK_F2){
        LOG(logINFO) << "F2 is pressed!";
        mq_.PutMessage(boost::shared_ptr<Message>(new SetStartPosEvent()));
    }
    //F3 设置结束点
    if(uVirtKey == VK_F3){
        LOG(logINFO) << "F3 is pressed!" ;
        mq_.PutMessage(boost::shared_ptr<Message>(new SetEndPosEvent()));
    }
    //F4 设置为正常播放状态
    if(uVirtKey == VK_F4){
        LOG(logINFO) << "F4 is pressed!" ;
        mq_.PutMessage(boost::shared_ptr<Message>(new ContinuePlayEvent()));
    }
    //F5 pause or resume
    if(uVirtKey == VK_F5){
        LOG(logINFO) << "F5 is pressed!" ;
        mq_.PutMessage(boost::shared_ptr<Message>(new PauseResumeEvent()));
    }
}

void MainWindow::OnPaint(CDCHandle dc)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(&ps);
    Graphics gfx(hdc);

    Gdiplus::Rect rect(rect_.left, rect_.top, rect_.right, rect_.bottom);

    LinearGradientBrush br(rect,	Color(42, 13, 4),	 Color(15, 0, 0), LinearGradientModeVertical  );
    Gdiplus::Status st = gfx.FillRectangle(&br, rect);

    Rect rc1 = rect;
    rc1.Inflate(-1, -1);
    LinearGradientBrush br1(rc1,	Color(72, 73, 74),	 Color(45, 45, 45), LinearGradientModeVertical  );
    Gdiplus::Status st1 = gfx.FillRectangle(&br1, rc1);
 
    Rect rc2 = rect;
    rc2.Inflate(-2, -2);
    LinearGradientBrush br2(rc2,	Color(42, 43, 44),	 Color(15, 15, 15), LinearGradientModeVertical  );
    Gdiplus::Status st2 = gfx.FillRectangle(&br2,rc2);  

    //Image image(L"logo.png");
    Rect icon_rect(icon_rect_.left, icon_rect_.top, icon_rect_.right, icon_rect_.bottom);

	CGdiPlusBitmapResource logo(L"LING_LOGO");
    gfx.DrawImage(logo, icon_rect);

    //FontFamily fontFamily(L"Courier New");
    FontFamily fontFamily(L"宋体");

    Font font(&fontFamily, 12, FontStyleRegular, UnitPixel);
    SolidBrush solidBrush(Color(255, 255, 255)/*Color(230, 77, 77)Color(255, 0, 0, 255)*/);
    wchar_t string1[] = L"风语";
    gfx.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
    gfx.DrawString(string1, (INT)wcslen(string1), &font, PointF(26, 7), &solidBrush);

    EndPaint(&ps);
}
void MainWindow::DrawFileName(const std::wstring& file_path)
{
    InvalidateRect(file_name_rect_, TRUE);

    std::wstring file_name = file_path.substr(file_path.rfind(L"\\") + 1);
    const wchar_t* text = file_name.c_str();
    file_name_static_.SetFileName(text);

}
UINT MainWindow::OnNcHitTest(CPoint point)
{
    ScreenToClient(&point);
    if (point.x >= title_rect_.left && point.x <= title_rect_.right
        && point.y >= title_rect_.top && point.y <= title_rect_.bottom){
        return HTCAPTION;
    }
    else{
        return HTCLIENT;
    }
}

LRESULT MainWindow::OnCloseButtonClicked( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	LOG(logINFO) << __FUNCTION__ ;
    mq_.PutMessage(boost::shared_ptr<Message>(new TerminateEvent()));
    //after the fsm terminated, send back message to destory the window later
    return TRUE;
}

LRESULT MainWindow::OnPauseResumeButtonClicked( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	LOG(logINFO) << __FUNCTION__ ;
    mq_.PutMessage(boost::shared_ptr<Message>(new PauseResumeEvent()));
    return TRUE;
}
LRESULT MainWindow::OnCreate(LPCREATESTRUCT lpcs)
{
    return 0;
}


LRESULT MainWindow::OnDestroy()
{
    if( !UnregisterHotKey(m_hWnd, hotkey_)){
        LOG(logERROR) << "Fail to unregister the hot key!" ;
    }
    else{
        LOG(logINFO) << "Successfully unregister the key!" ;
    }
        
    PostQuitMessage(0);
    return 0;
}   


void MainWindow::OnTimer(UINT_PTR nIDEvent)
{
    mq_.PutMessage(boost::shared_ptr<Message>(new UpdatePosEvent()));
}
void MainWindow::OnDropFiles(HDROP hDropInfo)
{
    TCHAR fileName[256];
    ::DragQueryFile(hDropInfo, 0, fileName, 256);
    mq_.PutMessage(boost::shared_ptr<Message>(new OpenFileEvent(fileName)));
}

LRESULT MainWindow::OnOpenMediaFile(UINT , int menu_id, HWND)
{

    LOG(logINFO) << menu_id <<" " << path(media_files_[menu_id]) ;
    mq_.PutMessage(boost::shared_ptr<Message>(new OpenFileEvent(media_files_[menu_id])));
    return TRUE;
}

LRESULT MainWindow::OnAbout( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
    LOG(logINFO) << __FUNCTION__ ;
    CAboutDlg dlg; 
    dlg.DoModal(); 
    return TRUE;
}


LRESULT MainWindow::OnSetFileInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    FileInfo* p_file_info = reinterpret_cast<FileInfo*> (wParam);
    DrawFileName(p_file_info->file_);
    bar_.SetLength(lParam);
//    bar_.SetSections(p_file_info->sections_ );
    delete p_file_info;
    bHandled = TRUE;
    return TRUE;
}

LRESULT MainWindow::OnStartTimer (UINT uMsg, WPARAM, LPARAM, BOOL& bHandled)
{
    StartTimer();
    bHandled = TRUE;
    return TRUE;
}

LRESULT MainWindow::OnStopTImer  (UINT uMsg, WPARAM, LPARAM, BOOL& bHandled)
{
    StopTimer();
    bHandled = TRUE;
    return TRUE;
}

//LRESULT MainWindow::OnSetStartPos (UINT uMsg, WPARAM start_pos, LPARAM, BOOL& bHandled)
//{
//    bHandled = TRUE;
//    bar_.SetStartPos(start_pos);
//    return TRUE;
//}
//
//LRESULT MainWindow::OnSetEndPos   (UINT uMsg, WPARAM end_pos, LPARAM, BOOL& bHandled)
//{
//    bHandled = TRUE;
//    bar_.SetEndPos(end_pos);
//    return TRUE;
//}

LRESULT MainWindow::OnExitApp (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = TRUE;
    DestroyWindow();
    return TRUE;
}

LRESULT MainWindow::OnPinButtonClicked( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
    //如果可以直接获取到当前窗口topmost的状态，可以直接进行反转。
    //使用了这个多余的变量
    static bool topmost = true;
    if(topmost){
        SetWindowPos(HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
        topmost = false;
    }
    else{
        SetWindowPos(HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
        topmost = true;
    }
    pin_button_.SwapNormalHoverImage();

    return TRUE;
}
LRESULT MainWindow::OnMarkButtonClicked( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
    LOG(logINFO)<< __FUNCTION__ ;
    mq_.PutMessage(boost::shared_ptr<Message>(new SaveSectionEvent()));
    return TRUE;
}

LRESULT MainWindow::OnSetRepo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    p_repo_ = (char*)(wParam);
    return TRUE;
}

LRESULT  MainWindow::OnDrawBar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bar_.SendMessage(uMsg, wParam, lParam);
    return TRUE;
}

LRESULT MainWindow::OnContextMenu(HWND , CPoint point)
{
    LOG(logINFO) << "ContextMenu will be displayed" << std::endl;
    CPoint p = point;
    ::ScreenToClient(bar_.m_hWnd, &p);
    long pos = bar_.GetPos(p);
    mq_.PutMessage(boost::shared_ptr<Message>(new RequestContextMenuInfoEvent(pos, point.x, point.y)));
    return 0;
}
LRESULT MainWindow::OnShowContextMenuRes(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    media_files_.clear();

    HMENU hMenu = LoadMenu(_Module.GetModuleInstance(), L"LING_POP");
    hMenu = GetSubMenu (hMenu, 0) ;

    HMENU file_menu = ::CreateMenu(); 

    std::vector<path> media_files;
    GetMediaFiles(p_repo_, media_files);
    
    unsigned int id_count = ID_LAST_MEDIA_FILE - ID_FIRST_MEDIA_FILE + 1 ;
    if(id_count < media_files.size() ){
            LOG(logERROR) << "Warning: Not enoughe ID can be used for the media files." << std::endl;
    }

    unsigned int i = 0;
    for( std::vector<path>::const_iterator it = media_files.begin();
        it != media_files.end() && i < id_count; 
        ++it, ++i){
            int menu_id = ID_FIRST_MEDIA_FILE + i;
            AppendMenu (file_menu, MF_STRING, menu_id, it->filename().wstring().c_str());
            media_files_[menu_id] = it->wstring();
    }
    ::InsertMenu (hMenu, 0, MF_POPUP | MF_BYPOSITION, (UINT_PTR)file_menu, L"文件") ;

    ContextMenuInfo* pcmi = (ContextMenuInfo*)wParam;

    if(pcmi->mark_){
        ::EnableMenuItem(hMenu, IDB_MARK, MF_BYCOMMAND | MF_ENABLED);
    }
    else{
        ::EnableMenuItem(hMenu, IDB_MARK, MF_BYCOMMAND | MF_DISABLED);
    }
    TrackPopupMenu (hMenu, TPM_RIGHTBUTTON, pcmi->x_, pcmi->y_, 
        0, m_hWnd, NULL) ;
    delete pcmi;
    return 0;
}

LRESULT MainWindow::OnTaskbarBtnCreated(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    DWORD dwMajor = LOBYTE(LOWORD(GetVersion()));
    DWORD dwMinor = HIBYTE(LOWORD(GetVersion()));

    // Check that the Windows version is at least 6.1 (yes, Win 7 is version 6.1).
    if ( dwMajor > 6 || ( dwMajor == 6 && dwMinor > 0 ) ){
        taskbar_list_.Release();
        taskbar_list_.CoCreateInstance ( CLSID_TaskbarList );
    }
    return TRUE;

}