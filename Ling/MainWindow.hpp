#ifndef REPEATER_WIN
#define REPEATER_WIN
#include "Bar.hpp"
#include "CoolButton.hpp"
#include "UiInterface.hpp"
#include "Fsm.hpp"
#include "Ui.hpp"
#include "FileNameStatic.hpp"
#include "MessageQueue.hpp"
#include "Utils.hpp"

///////////////////////////////////////////
//How to define this min & max value, maybe conflict with the ID in resource.h?
#define ID_FIRST_MEDIA_FILE 3000
#define ID_LAST_MEDIA_FILE  4000
///////////////////////////////////////////--------->>>>>>>>>>>>>>

typedef CWinTraits<WS_POPUP | WS_SYSMENU /*| WS_CAPTION   | WS_BORDER*/|WS_VISIBLE, WS_EX_TOPMOST /*| WS_EX_TOOLWINDOW*/> RepeaterWinTraits;
class MainWindow: 
    public CWindowImpl< MainWindow, CWindow, RepeaterWinTraits>,
    //public CMessageFilter,
    public Ui< MainWindow>
{
public:
    DECLARE_WND_CLASS(_T("REPEATER_WIN"))
    /*virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
        LOG(logINFO) << std::hex << __FUNCTION__ << " received msg " << pMsg->message << std::dec ;
		return FALSE;
	}*/

    BEGIN_MSG_MAP_EX(RepeaterWindow)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_NCHITTEST(OnNcHitTest)
        MSG_WM_PAINT(OnPaint)
        MSG_WM_DROPFILES(OnDropFiles)
        MSG_WM_HOTKEY(OnHotKey)
        MSG_WM_TIMER(OnTimer)

        MSG_WM_CONTEXTMENU(OnContextMenu)
        COMMAND_ID_HANDLER(IDB_CLOSE, OnCloseButtonClicked)
        COMMAND_ID_HANDLER(IDM_PAUSE_RESUME, OnPauseResumeButtonClicked)
        COMMAND_ID_HANDLER(IDM_ABOUT, OnAbout)
        COMMAND_RANGE_HANDLER_EX(ID_FIRST_MEDIA_FILE, ID_LAST_MEDIA_FILE, OnOpenMediaFile)
        COMMAND_ID_HANDLER(IDB_PIN, OnPinButtonClicked)
        COMMAND_ID_HANDLER(IDB_MARK, OnMarkButtonClicked)
        //自定义消息
        MESSAGE_HANDLER(WM_STARTTIMER,   OnStartTimer)
        MESSAGE_HANDLER(WM_STOPTIMER,    OnStopTImer)
        MESSAGE_HANDLER(WM_SETFILEINFO,  OnSetFileInfo)
        MESSAGE_HANDLER(WM_EXITAPP,      OnExitApp)
        MESSAGE_HANDLER(WM_SETREPO,      OnSetRepo)
        MESSAGE_HANDLER(WM_DRAWBAR,      OnDrawBar)
        MESSAGE_HANDLER(WM_CONTEXTMENUINFO, OnShowContextMenuRes)

        //
        MESSAGE_HANDLER(TASKBAR_CREATE_MESSAGE, OnTaskbarBtnCreated)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()


    //TODO: remove this friend
    friend class ProgressBar;
    MainWindow(MessageQueue& mq);

    void OnHotKey(int nHotKeyID, UINT uModifiers, UINT uVirtKey);    
    void OnTimer(UINT_PTR nIDEvent);
    LRESULT OnContextMenu(HWND WPARAM, CPoint point);
    void OnDropFiles(HDROP hDropInfo);
    void OnPaint(CDCHandle dc);
    UINT OnNcHitTest(CPoint point);

    LRESULT OnCloseButtonClicked      ( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
    LRESULT OnPauseResumeButtonClicked( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
    LRESULT OnAbout                   ( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
    LRESULT OnPinButtonClicked        ( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
    LRESULT OnMarkButtonClicked        ( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
    LRESULT OnCreate(LPCREATESTRUCT lpcs);
    LRESULT OnDestroy();
    LRESULT OnOpenMediaFile(UINT, int, HWND);

    //自定义消息
    LRESULT OnSetFileInfo (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnStartTimer  (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnStopTImer   (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    //LRESULT OnSetStartPos (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    //LRESULT OnSetEndPos   (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnExitApp     (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSetRepo     (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDrawBar     (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnShowContextMenuRes(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnTaskbarBtnCreated(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    void DrawFileName(const std::wstring& file_name);
    void StartTimer(){SetTimer(timerId_, 500); }
    void StopTimer(){KillTimer(timerId_);}



private:
    ProgressBar bar_;
    CRect rect_, bar_rect_, close_rect_, icon_rect_, title_rect_, file_name_rect_;
    ATOM hotkey_;

    MessageQueue& mq_;
    LingJson json_;

    static const int timerId_ = 199;
    CoolButton close_button_;
    std::map<int, std::wstring> media_files_;
    const char* p_repo_;
    FileNameStatic file_name_static_;

    CoolButton a_button_, b_button_;
    CRect a_rect_, b_rect_;

    CoolButton pin_button_;
    CRect pin_rect_;

    CComPtr<ITaskbarList3> taskbar_list_;
    static const UINT TASKBAR_CREATE_MESSAGE;

};
#endif
