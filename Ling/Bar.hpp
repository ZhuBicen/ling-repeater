#ifndef PROGRESS_BAR
#define PROGRESS_BAR

#include "Precompiled.hpp"
#include "Player.hpp"
#include "Utils.hpp"
#include "Ui.hpp"
class CMainDlg;

class ProgressBar:public CWindowImpl<ProgressBar, CWindow, CControlWinTraits>
{
public:
    DECLARE_WND_CLASS(_T("REPEATER_WIN"))
    
    BEGIN_MSG_MAP_EX(ProgressBar)
        MSG_WM_PAINT(OnPaint)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_SETCURSOR(OnSetCursor)
        MSG_WM_LBUTTONDOWN(OnButtonDown )
        MESSAGE_HANDLER(WM_DRAWBAR,      OnDrawBar)
    END_MSG_MAP()


    ProgressBar(CMainDlg& main_window);

    LRESULT OnSetCursor(HWND , UINT , UINT );
    int OnCreate(LPCREATESTRUCT lpCreateStruct);
    void OnPaint(CDCHandle dc);

    LRESULT OnDestroy();
    LRESULT OnButtonDown( UINT WPARAM, CPoint point);

    void SetLength(long length);

    void UpdateNormalPlayingPosition(long pre_pos, long new_pos);
    void UpdateRepeatedPlayingPosition(long pre_pos, long new_pos);

    static int CalculateX(long pos, long length, int width){
        return static_cast<int> (ceil( width * static_cast<double> (pos) / length));
    }
    LRESULT  OnDrawBar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    void DrawNormal(const PaintInfo* pi);
    void DrawRepeat(const PaintInfo* pi);
    void DrawSection(const Section& sec);
    long GetPos(CPoint p);
private:
    void DrawBackground(Graphics& gfx, const Sections& sections);
    void DrawRectangle(Graphics& gfx, Brush* brush, long from, long to);
    void DrawLine(Graphics& gfx, Brush* brush, long from, long to);
private:
    CMainDlg& main_window_;
    Rect rect0_, rect1_, rect2_;

    //UI style related
    /*LinearGradientBrush*/SolidBrush* play_brush_, *repeat_brush_;
    SolidBrush* background_brush_, *section_brush_;;

    //file info
    long length_;
};
#endif
