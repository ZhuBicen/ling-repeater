#ifndef HOTKEYEDIT_HPP
#define HOTKEYEDIT_HPP 
#include "Precompiled.hpp"

class HotkeyEdit: public CWindowImpl<HotkeyEdit, CEdit>
{
public:
    BEGIN_MSG_MAP(HotkeyEdit)
       MESSAGE_HANDLER(WM_KEYDOWN, OnKeydown)
    END_MSG_MAP()
    LRESULT OnKeydown(UINT,  WPARAM wParam, LPARAM , BOOL& bHandled){
        
        return TRUE;
    }
};

#endif
