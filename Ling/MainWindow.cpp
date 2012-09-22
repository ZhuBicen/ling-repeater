#include "Precompiled.hpp"
#include "MainWindow.hpp"
#include "CGdiPlusBitmap.h"
#include "AboutDlg.h"
#include "AtlDlgs.h"
#include "SettingDlg.hpp"
#include "Message.hpp"

const UINT CMainDlg::TASKBAR_CREATE_MESSAGE = RegisterWindowMessage ( _T("TaskbarButtonCreated") );
LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    // center the dialog on the screen
    CenterWindow();
    SetWindowText(L"风语");
    SetWindowPos(HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

    // set icons
    HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
    SetIcon(hIcon, TRUE);
    HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
    SetIcon(hIconSmall, FALSE);

    // register object for message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->AddMessageFilter(this);
    pLoop->AddIdleHandler(this);

    UIAddChildWindowContainer(m_hWnd);

    RECT dlgRect;
    ::GetClientRect(m_hWnd, &dlgRect);
    title_rect_.top = title_rect_.left = 0;
    title_rect_.right = dlgRect.right;

    HWND hwnd = ::GetDlgItem(this->m_hWnd, IDC_PROGRESSBAR);
    RECT rect;
    ::GetClientRect(hwnd, &rect);
    
    title_rect_.bottom = rect.bottom - rect.top;

    POINT leftTop;
    leftTop.x = rect.left;
    leftTop.y = rect.top;

    POINT rightBottom;
    rightBottom.x = rect.right;
    rightBottom.y = rect.bottom;

    ::ClientToScreen(hwnd, &leftTop);
    ::ClientToScreen(hwnd, &rightBottom);

    ::ScreenToClient(this->m_hWnd, &leftTop);
    ::ScreenToClient(this->m_hWnd, &rightBottom);
    ::ShowWindow(hwnd, SW_HIDE);

    bar_.Create(*this, CRect(leftTop.x, leftTop.y, rightBottom.x, rightBottom.y), TEXT("PROGRESS_BAR"));
    DragAcceptFiles(TRUE);
    if (!RegisterHotkeys()){
        LOG(logERROR) << "ERROR Can't register the hotkey";
    }
    //显示文件名
    file_name_static_.SubclassWindow(GetDlgItem(IDC_FILENAME));
    close_button_.SubclassWindow(GetDlgItem(IDOK));
    play_button_.SubclassWindow(GetDlgItem(IDB_MENU));
    theme_button_.SubclassWindow(GetDlgItem(IDC_THEME));
    time_label_.SubclassWindow(GetDlgItem(IDC_TIME));

    redrawers_.push_back(this);
    redrawers_.push_back(&bar_);
    redrawers_.push_back(&file_name_static_);
    redrawers_.push_back(&close_button_);
    redrawers_.push_back(&theme_button_);
    redrawers_.push_back(&play_button_);
    redrawers_.push_back(&time_label_);
    return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    // unregister message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->RemoveMessageFilter(this);
    pLoop->RemoveIdleHandler(this);

    return 0;
}

LRESULT CMainDlg::OnAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CAboutDlg dlg;
    dlg.DoModal();
    return 0;
}

LRESULT CMainDlg::OnSetting ( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
    SettingDlg dlg;
    dlg.DoModal();
    return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	LOG(logINFO) << __FUNCTION__ ;
    mq_.PutMessage(boost::shared_ptr<Message>(EventFactory::makeEvent(EVENT_ID_TERMINATE)));
    //after the fsm terminated, send back message to destory the window later
    return TRUE;
}

void CMainDlg::CloseDialog(int nVal)
{
    DestroyWindow();
    ::PostQuitMessage(nVal);
}


void CMainDlg::OnDropFiles(HDROP hDropInfo)
{
    TCHAR fileName[256];
    ::DragQueryFile(hDropInfo, 0, fileName, 256);
    mq_.PutMessage(boost::shared_ptr<Message>(EventFactory::makeOpenFileEvent(fileName)));
}
void CMainDlg::OnHotKey(int nHotKeyID, UINT uModifiers, UINT uVirtKey)
{
    LOG(logINFO) << __FUNCTION__ ;
    //F2 设置开始点
    if(uVirtKey == VK_F2){
        LOG(logINFO) << "F2 is pressed!";
        mq_.PutMessage(boost::shared_ptr<Message>(EventFactory::makeEvent(EVENT_ID_SET_START_POS)));
    }
    //F3 设置结束点
    if(uVirtKey == VK_F3){
        LOG(logINFO) << "F3 is pressed!" ;
        mq_.PutMessage(boost::shared_ptr<Message>(EventFactory::makeEvent(EVENT_ID_SET_END_POS)));
    }
    //F4 设置为正常播放状态
    if(uVirtKey == VK_F4){
        LOG(logINFO) << "F4 is pressed!" ;
        mq_.PutMessage(boost::shared_ptr<Message>(EventFactory::makeEvent(EVENT_ID_CONTINUE_PLAY)));
    }
    //F5 pause or resume
    if(uVirtKey == VK_F5){
        LOG(logINFO) << "F5 is pressed!" ;
        mq_.PutMessage(boost::shared_ptr<Message>(EventFactory::makeEvent(EVENT_ID_PAUSE_RESUME)));
    }
}
void CMainDlg::OnTimer(UINT_PTR nIDEvent)
{
    mq_.PutMessage(boost::shared_ptr<Message>(EventFactory::makeEvent(EVENT_ID_UPDATE_POS)));
}
LRESULT CMainDlg::OnStartTimer (UINT uMsg, WPARAM, LPARAM, BOOL& bHandled)
{
    SetTimer(timerId_, 500);
    bHandled = TRUE;
    return TRUE;
}

LRESULT CMainDlg::OnStopTImer  (UINT uMsg, WPARAM, LPARAM, BOOL& bHandled)
{
    KillTimer(timerId_);
    bHandled = TRUE;
    return TRUE;
}
LRESULT CMainDlg::OnContextMenu(HWND , CPoint point)
{
    LOG(logINFO) << "ContextMenu will be displayed" << std::endl;
    CPoint p = point;
    ::ScreenToClient(bar_.m_hWnd, &p);
    long pos = bar_.GetPos(p);
    mq_.PutMessage(boost::shared_ptr<Message>(EventFactory::makeRequestContextMenuInfoEvent(pos, point.x, point.y)));
    return 0;
}
LRESULT CMainDlg::OnTaskbarBtnCreated(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CMainDlg::OnSetFileInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    FileInfo* p_file_info = reinterpret_cast<FileInfo*> (wParam);
    file_name_static_.SetText(p_file_info->file_);
    bar_.SetLength(lParam);
    delete p_file_info;
    bHandled = TRUE;
    return TRUE;
}

LRESULT CMainDlg::OnExitApp (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = TRUE;
    DestroyWindow();
    return TRUE;
}

LRESULT CMainDlg::OnSetConfInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    //Should use scoped_ptr?
    ConfInfo* pci = reinterpret_cast<ConfInfo*>(wParam);
    repo_ = pci->repo_;
    hotkeys_ = pci->hotkeys_;
    delete pci;
    return TRUE;
}

LRESULT  CMainDlg::OnDrawBar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    long current_pos, length;
    current_pos = INVALID_POS;
    length = INVALID_POS;
    if (lParam == 0){
        const PaintInfo* pi = (PaintInfo*)wParam;
        current_pos = pi->current_pos_;
        length = pi->length_;

    }else if (lParam == 1){
        const PaintUpdateInfo* pui= (PaintUpdateInfo*)wParam;
        if (pui->color_ == 0){
            current_pos = pui->current_pos_;
            length = pui->length_;
        }
    }else{
        LOG(logERROR) << "Illegal lParam " << lParam ;
    }
    if (current_pos != INVALID_POS && length != INVALID_POS){
        time_label_.SetText(GetTime(length - current_pos));
    }
    bar_.SendMessage(uMsg, wParam, lParam);
    return TRUE;
}
LRESULT CMainDlg::OnShowMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled)
{
    HWND button = GetDlgItem(wID);
    RECT rect;
    ::GetClientRect(button, &rect);
    POINT p;
    p.x = rect.left;
    p.y = rect.bottom;
    ::ClientToScreen(button, &p);
    HMENU hMenu = LoadMenu(_Module.GetModuleInstance(), L"LING_POP");
    hMenu = GetSubMenu (hMenu, 2) ;

    HMENU file_menu = ::CreateMenu(); 

    std::vector<path> media_files;
    GetMediaFiles(repo_, media_files);
    
    unsigned int id_count = ID_LAST_MEDIA_FILE - ID_FIRST_MEDIA_FILE + 1 ;
    if(id_count < media_files.size() ){
            LOG(logERROR) << "Warning: Not enoughe ID can be used for the media files." << std::endl;
    }

    if (!media_files.empty()){
        unsigned int i = 0;
        for( std::vector<path>::const_iterator it = media_files.begin();
             it != media_files.end() && i < id_count; 
             ++it, ++i){
            int menu_id = ID_FIRST_MEDIA_FILE + i;
            AppendMenu (file_menu, MF_STRING, menu_id, it->filename().wstring().c_str());
            media_files_[menu_id] = it->wstring();
        }
        ::InsertMenu (hMenu, 1, MF_POPUP | MF_BYPOSITION, (UINT_PTR)file_menu, L"文件") ;
    }
    TrackPopupMenu (hMenu, TPM_RIGHTBUTTON, p.x, p.y, 0, m_hWnd, NULL) ;
    return TRUE;
}
    
    
LRESULT CMainDlg::OnShowContextMenuRes(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    media_files_.clear();

    HMENU hMenu = LoadMenu(_Module.GetModuleInstance(), L"LING_POP");
    hMenu = GetSubMenu (hMenu, 0) ;
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

void CMainDlg::OnPaint(CDCHandle dc)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(&ps);
    Graphics gfx(hdc);
    RECT dlgRect ;
    GetClientRect(&dlgRect);
    Gdiplus::Rect rect(dlgRect.left, dlgRect.top, dlgRect.right, dlgRect.bottom);
    SolidBrush br(bg_color_);
    Gdiplus::Status st = gfx.FillRectangle(&br, rect);

	CGdiPlusBitmapResource logo(L"LING_LOGO");
    Rect icon_rect(icon_rect_.left, icon_rect_.top, icon_rect_.right, icon_rect_.bottom);
    gfx.DrawImage(logo, icon_rect);
    //Rect rc1 = rect;
    //rc1.Inflate(-1, -1);
    //LinearGradientBrush br1(rc1,	Color(72, 73, 74),	 Color(45, 45, 45), LinearGradientModeVertical  );
    //Gdiplus::Status st1 = gfx.FillRectangle(&br1, rc1);
 
    //Rect rc2 = rect;
    //rc2.Inflate(-2, -2);
    //LinearGradientBrush br2(rc2,	Color(42, 43, 44),	 Color(15, 15, 15), LinearGradientModeVertical  );
    //Gdiplus::Status st2 = gfx.FillRectangle(&br2,rc2); 
    EndPaint(&ps);
}

void CMainDlg::Redraw()
{
    bg_color_ = Theme::Get()->BgColor();
    InvalidateRect(NULL);
}
LRESULT CMainDlg::OnMarkButtonClicked( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
    LOG(logINFO)<< __FUNCTION__ ;
	mq_.PutMessage(boost::shared_ptr<Message>(EventFactory::makeEvent(EVENT_ID_SAVE_SECTION)));
    return TRUE;
}
LRESULT CMainDlg::OnPinButtonClicked( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
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
    //pin_button_.SwapNormalHoverImage();

    return TRUE;
}
LRESULT CMainDlg::OnOpenMediaFile(UINT , int menu_id, HWND)
{

    LOG(logINFO) << menu_id <<" " << path(media_files_[menu_id]) ;
    mq_.PutMessage(boost::shared_ptr<Message>(EventFactory::makeOpenFileEvent(media_files_[menu_id])));
    return TRUE;
}
LRESULT CMainDlg::OnPauseResumeButtonClicked( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	LOG(logINFO) << __FUNCTION__ ;
    mq_.PutMessage(boost::shared_ptr<Message>(EventFactory::makeEvent(EVENT_ID_PAUSE_RESUME)));
    return TRUE;
}
UINT CMainDlg::OnNcHitTest(CPoint point)
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

LRESULT CMainDlg::OnChangeTheme(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled)
{
    Theme::Change();
    for(std::vector<Theme::Redrawer*>::iterator ci = redrawers_.begin();
        ci != redrawers_.end(); ci++){
            (*ci)->Redraw();
    }
    return TRUE;
}
LRESULT CMainDlg::OnOpen(WORD, WORD, HWND, BOOL&)
{
    CFileDialog cFilePath( TRUE,NULL,NULL,OFN_SHAREAWARE,_T("MP3 Files\0*.mp3\0 AllFiles (*.*)\0*.*\0") );
    WCHAR strBuffer[1024] = {0};
    cFilePath.m_ofn.lpstrFile = strBuffer;
    cFilePath.m_ofn.nMaxFile = 1024;
    if( cFilePath.DoModal() == IDOK )
    {
        LPWSTR lpFilePath = cFilePath.m_ofn.lpstrFile;
        Player player(NULL);
        if (player.IsSupport(lpFilePath)){
            mq_.PutMessage(boost::shared_ptr<Message>(EventFactory::makeOpenFileEvent(lpFilePath)));
        }else{
            ::MessageBox(m_hWnd, L"不能识别的文件！", L"ERROR", MB_OK);
        }
    }

    return TRUE;
}

bool CMainDlg::RegisterHotkeys(){
    hotkey_ = GlobalAddAtom(TEXT("ZHUBICEN_REPEATER"));
    if (!RegisterHotKey(m_hWnd, hotkey_, 0, VK_F1)){
        LOG(logERROR) << "Can't register the hot key F1" ;
        return false;
    }
    if (!RegisterHotKey(m_hWnd, hotkey_ + 1, 0, VK_F2)){
        LOG(logERROR) << "Can't register the hot key F2" ;
        return false;
    }
    if (!RegisterHotKey(m_hWnd, hotkey_ + 2, 0, VK_F3)){
        LOG(logERROR) << "Can't register the hot key F3" ;
        return false;        
    }
    if (!RegisterHotKey(m_hWnd, hotkey_ + 3, 0, VK_F4)){
        LOG(logERROR) << "Can't register the hot key F4" ;
        return false;
    }
    if (!RegisterHotKey(m_hWnd, hotkey_ + 4, 0, VK_F5)){
        LOG(logERROR) << "Can't register the hot key F5" ;
        return false;
    }
}
bool CMainDlg::UnregisterHotkeys(){
    for (int i = 0; i < 5; i++){
        ::UnregisterHotKey(m_hWnd, hotkey_ + i);
    }
    return true;
}
