#pragma once
#include "PreCompiled.hpp"
#include "Theme.hpp"

class CoolLabel: public CWindowImpl<CoolLabel, CStatic>, public Theme::Redrawer
{
public:
    BEGIN_MSG_MAP_EX(CoolLabel)
        MSG_WM_PAINT(OnPaint)
    END_MSG_MAP()

    void OnPaint(CDCHandle dc)
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(&ps);
        Graphics gfx(hdc);

        RECT dlgRect ;
        GetClientRect(&dlgRect);
        Gdiplus::Rect rect(dlgRect.left, dlgRect.top, dlgRect.right, dlgRect.bottom);
        SolidBrush br(bg_color_);
        Gdiplus::Status st = gfx.FillRectangle(&br, rect);


        FontFamily fontFamily(L"����");
        Font font(&fontFamily, 12, FontStyleRegular, UnitPixel);
        SolidBrush solidBrush(Color(255, 255, 255));
        gfx.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
        const wchar_t* text = file_name_.c_str();
        gfx.DrawString(text, (INT)wcslen(text), &font, PointF(0,0), &solidBrush);

        EndPaint(&ps);
    }
    CoolLabel(){
        bg_color_ = Theme::Get()->BgColor();
    }
    void Redraw(){
        bg_color_ = Theme::Get()->BgColor();
        InvalidateRect(NULL);
    }
    void SetText(const std::wstring& file_name){
        file_name_ = file_name;
        CRect rect;
        GetClientRect(&rect);
        InvalidateRect( rect, TRUE);

    }
private:
    std::wstring file_name_;
    Color bg_color_;

};
