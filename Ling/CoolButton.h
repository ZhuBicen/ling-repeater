#ifndef LING_COOLBUTTON_HPP_INCLUDED
#define LING_COOLBUTTON_HPP_INCLUDED
#include "Precompiled.hpp"

class CoolButton : public CWindowImpl<CoolButton, CButton>, public COwnerDraw<CoolButton>
{
    bool _bMouseTrack;
    Rect rect_;
public:
    BEGIN_MSG_MAP_EX(CoolButton)
        CHAIN_MSG_MAP_ALT(COwnerDraw<CoolButton>, 1)
        MESSAGE_HANDLER(WM_MOUSEHOVER, OnMouseHover)
        MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
        MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
        DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()
    CoolButton(void);
    ~CoolButton(void);
    void DrawItem(LPDRAWITEMSTRUCT lpdis);
    LONG OnMouseHover(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
    {
        std::cout << "OnMouseHover!" << std::endl;
        HDC dc = GetDC();
        Graphics gfx(dc);
        Image image(L"close_hover.png");
        gfx.DrawImage(&image, rect_);
        return 0;
    }
    LONG OnMouseMove(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
    {
        if (_bMouseTrack)
        {
            TRACKMOUSEEVENT csTME;
            csTME.cbSize = sizeof (csTME);
            csTME.dwFlags = TME_LEAVE|TME_HOVER;
            csTME.hwndTrack = m_hWnd ;
            csTME.dwHoverTime = 10;  
            ::_TrackMouseEvent (&csTME);
            _bMouseTrack = false;
        }
        return 0;
    }
    LONG OnMouseLeave(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
    {
        _bMouseTrack = true ;
        Invalidate();
        return 0;
    }
};
#endif