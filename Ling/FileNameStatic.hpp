#pragma once
#include "PreCompiled.hpp"

class FileNameStatic: public CWindowImpl<FileNameStatic, CStatic>
{
public:
    BEGIN_MSG_MAP_EX(FileNameStatic)
        MSG_WM_PAINT(OnPaint)
    END_MSG_MAP()
    void OnPaint(CDCHandle dc)
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(&ps);
        Graphics gfx(hdc);

        FontFamily fontFamily(L"ËÎÌו");
        Font font(&fontFamily, 12, FontStyleRegular, UnitPixel);
        SolidBrush solidBrush(Color(255, 255, 255));
        gfx.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
        const wchar_t* text = file_name_.c_str();
        gfx.DrawString(text, (INT)wcslen(text), &font, PointF(0,0), &solidBrush);

        EndPaint(&ps);
    }
    void SetFileName(const std::wstring& file_name){
        file_name_ = file_name;
        CRect rect;
        GetClientRect(&rect);
        InvalidateRect( rect, TRUE);

    }
private:
    std::wstring file_name_;

};
