// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "HotkeyEdit.hpp"
class SettingDlg : public CDialogImpl<SettingDlg>
{
public:
	enum { IDD = IDD_SETTINGS };

	BEGIN_MSG_MAP(CAboutDlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
        COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()


	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        CenterWindow(GetParent());
        hotkey_edits_[0].SubclassWindow(GetDlgItem(IDC_EDIT0));
        hotkey_edits_[1].SubclassWindow(GetDlgItem(IDC_EDIT1));
        hotkey_edits_[2].SubclassWindow(GetDlgItem(IDC_EDIT2));
        hotkey_edits_[3].SubclassWindow(GetDlgItem(IDC_EDIT3));        
        return TRUE;
    }

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        EndDialog(wID);
        return 0;
    }
private:
    HotkeyEdit hotkey_edits_[4];
};
