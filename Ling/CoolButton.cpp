#include "Precompiled.hpp"
#include "CoolButton.hpp"

CoolButton::CoolButton(std::wstring normal, std::wstring hover):
 _bMouseTrack(true)
{
    //within debug mode, this var will be initilized as true, but not release mode
    //so, assign a true explictly.
     normal_image_.Load(normal.c_str());
     hover_image_.Load(hover.c_str());
     bg_color_ = Theme::Get()->BgColor();
}

CoolButton::~CoolButton(void)
{
}
void CoolButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    LOG(logDEBUG1) << "CoolButton::DrawItem()" ;
    HDC hdc = lpDrawItemStruct->hDC;
    RECT rect = lpDrawItemStruct->rcItem;
    rect_ = Rect(rect.left, rect.top, rect.right, rect.bottom);

    int width = rect.right - rect.left;
    int height = (rect.bottom - rect.top) ;
    int min = height < width ? height : width;

    Rect rc(rect.left, rect.top,  min, min );
    rect2_ = rc;

    Graphics gfx(hdc);
    DrawBackground(gfx);

    if( lpDrawItemStruct->itemState & ODS_FOCUS ){
        DrawBackground(gfx);
        gfx.DrawImage(normal_image_, rect2_);
    }
    if( lpDrawItemStruct->itemState & ODS_SELECTED){
        DrawBackground(gfx);
        gfx.DrawImage(hover_image_, rect2_);
     }
     else{
         DrawBackground(gfx);
         gfx.DrawImage(normal_image_, rect2_);
     }
    
    
}
LONG CoolButton::OnMouseHover(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{
    LOG(logDEBUG1) << "OnMouseHover!" ;
    HDC dc = GetDC();
    Graphics gfx(dc);
    DrawBackground(gfx);
	gfx.DrawImage(hover_image_, rect2_);
    return 0;
}

void CoolButton::DrawBackground(Graphics& gfx)
{
    SolidBrush blackBrush(bg_color_);
    gfx.FillRectangle(&blackBrush, rect_);

}