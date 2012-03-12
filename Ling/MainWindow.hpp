// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Ui.hpp"
#include "Bar.hpp"
#include "MessageQueue.hpp"
#include "FileNameStatic.hpp"
#include "CoolButton.hpp"
#include "resource.h"
#include "Theme.hpp"
///////////////////////////////////////////
//How to define this min & max value, maybe conflict with the ID in resource.h?
#define ID_FIRST_MEDIA_FILE 3000
#define ID_LAST_MEDIA_FILE  4000
///////////////////////////////////////////--------->>>>>>>>>>>>>>

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler,
        public Ui< CMainDlg>,
        public Theme::Redrawer

{
public:
    CMainDlg(MessageQueue& mq):
      mq_(mq), bar_(*this), icon_rect_(1, 1, 22, 22), 
      bg_color_(Theme::Get()->BgColor()), 
        close_button_(L"CLOSE_NORMAL", L"CLOSE_HOVER"),
        play_button_(L"PLAY_NORMAL", L"PLAY_HOVER"),
        theme_button_(L"THEME_NORMAL", L"THEME_NORMAL"){
    }
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
        return CWindow::IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MSG_WM_NCHITTEST(OnNcHitTest)
		COMMAND_ID_HANDLER(IDOK, OnOK)
        COMMAND_ID_HANDLER(IDB_CLOSE, OnOK)
        COMMAND_ID_HANDLER(IDM_OPEN, OnOpen)

        MSG_WM_DROPFILES(OnDropFiles)
        MSG_WM_HOTKEY(OnHotKey)
        MSG_WM_TIMER(OnTimer)
        MSG_WM_PAINT(OnPaint)

        MSG_WM_CONTEXTMENU(OnContextMenu)
        COMMAND_ID_HANDLER(IDM_PAUSE_RESUME, OnPauseResumeButtonClicked)
        COMMAND_ID_HANDLER(IDM_ABOUT, OnAbout)
        COMMAND_ID_HANDLER(IDM_SETTING, OnSetting)
        COMMAND_RANGE_HANDLER_EX(ID_FIRST_MEDIA_FILE, ID_LAST_MEDIA_FILE, OnOpenMediaFile)
        COMMAND_ID_HANDLER(IDB_PIN, OnPinButtonClicked)
        COMMAND_ID_HANDLER(IDB_MARK, OnMarkButtonClicked)
        COMMAND_ID_HANDLER(IDC_THEME, OnChangeTheme)
        COMMAND_ID_HANDLER(IDB_MENU, OnShowMenu)
        //自定义消息
        MESSAGE_HANDLER(WM_STARTTIMER,   OnStartTimer)
        MESSAGE_HANDLER(WM_STOPTIMER,    OnStopTImer)
        MESSAGE_HANDLER(WM_SETFILEINFO,  OnSetFileInfo)
        MESSAGE_HANDLER(WM_EXITAPP,      OnExitApp)
        MESSAGE_HANDLER(WM_SETCONFINFO,      OnSetRepo)
        MESSAGE_HANDLER(WM_DRAWBAR,      OnDrawBar)
        MESSAGE_HANDLER(WM_CONTEXTMENUINFO, OnShowContextMenuRes)
        MESSAGE_HANDLER(TASKBAR_CREATE_MESSAGE, OnTaskbarBtnCreated)
        REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

    void OnHotKey(int nHotKeyID, UINT uModifiers, UINT uVirtKey);    
    void OnTimer(UINT_PTR nIDEvent);
    LRESULT OnContextMenu(HWND WPARAM, CPoint point);
    void OnDropFiles(HDROP hDropInfo);
    void OnPaint(CDCHandle dc);
    UINT OnNcHitTest(CPoint point);

    LRESULT OnCloseButtonClicked      ( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
    LRESULT OnPauseResumeButtonClicked( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
    LRESULT OnAbout                   ( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
    LRESULT OnSetting                 ( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );    
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
    LRESULT OnChangeTheme(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled);
    LRESULT OnShowMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled);    

//  Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnOpen(WORD, WORD, HWND, BOOL&);
	void CloseDialog(int nVal);

    //TODO: remove this friend
    friend class ProgressBar;

    //implement redrawer
    void Redraw();
    bool RegisterHotkeys();
    bool UnregisterHotkeys();
private:
    Color bg_color_;
    std::map<int, std::wstring> media_files_;
    CRect title_rect_, icon_rect_;
    const char* p_repo_;
    static const int timerId_ = 199;
    MessageQueue& mq_;
    ProgressBar bar_;
    ATOM hotkey_;
    CComPtr<ITaskbarList3> taskbar_list_;
    static const UINT TASKBAR_CREATE_MESSAGE;// = RegisterWindowMessage ( _T("TaskbarButtonCreated") );
    CoolButton play_button_, close_button_, theme_button_;
    CoolLabel file_name_static_, time_label_;

    std::vector<Theme::Redrawer*> redrawers_;
};
