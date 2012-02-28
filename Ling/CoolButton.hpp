#ifndef LING_COOLBUTTON_HPP_INCLUDED
#define LING_COOLBUTTON_HPP_INCLUDED
#include "Precompiled.hpp"
#include "CGdiPlusBitmap.h"
#include "log.hpp"

class CoolButton : public CWindowImpl<CoolButton, CButton>, public COwnerDraw<CoolButton>
{
    bool _bMouseTrack;
    Rect rect_;
    CGdiPlusBitmapResource normal_image_, hover_image_;
public:
    BEGIN_MSG_MAP_EX(CoolButton)
        CHAIN_MSG_MAP_ALT(COwnerDraw<CoolButton>, 1)
        MESSAGE_HANDLER(WM_MOUSEHOVER, OnMouseHover)
        MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
        MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
        //MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
        DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()
    CoolButton(std::wstring normal, std::wstring hover);
    ~CoolButton(void);
    void DrawItem(LPDRAWITEMSTRUCT lpdis);
    LONG OnMouseHover(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);

    LONG OnMouseMove(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
    {
        LOG(logDEBUG1) << __FUNCTION__ ;
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
        LOG(logDEBUG1) << __FUNCTION__ ;
        _bMouseTrack = true ;
        Invalidate();
        return 0;
    }
    //LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    //{
    //    //InvalidateRect(NULL, FALSE) ;
    //    return 1;   // no background painting needed
    //}
    void SwapNormalHoverImage()
    {
        SwapGdiPlusBitmap(normal_image_, hover_image_);
        InvalidateRect(NULL, FALSE);
    }
private:
    void DrawBackground(Graphics& gfx);
};
#endif