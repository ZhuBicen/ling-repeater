#include "Precompiled.hpp"
#include "Bar.hpp"
#include "MainWindow.hpp"
#include "UiInterface.hpp"

ProgressBar::ProgressBar(MainWindow& main_window)
    :main_window_(main_window), length_(0)//,
{}

int ProgressBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    RECT rect;
    GetClientRect(&rect);
    Rect rc(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
    rect2_ = rc;
    rc.Inflate(-1, -1);
    rect1_ = rc;
    rc.Inflate(-1, -1);
    rect0_ = rc;
    //���ڵ��⣬�ֱ�Ϊrect0_, rect1_, rect2_
    play_brush_ = new LinearGradientBrush(rect0_, Color(230, 77, 77), Color(188, 31, 31), LinearGradientModeVertical);
    repeat_brush_ = new LinearGradientBrush(rect0_, Color(255, 203, 127), Color(214, 112, 0), LinearGradientModeVertical);
    background_brush_ = new SolidBrush(Color(68, 68, 68));
    section_brush_ = new SolidBrush(Color(24, 141, 211));
    
    return TRUE;

}
LRESULT  ProgressBar::OnDrawBar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    LOG(logINFO)<< __FUNCTION__ << std::hex << " (uMsg = 0x" << uMsg <<", wParam = 0x" << wParam << ", lParam = 0x" << lParam << ")" << std:: dec;
    if( lParam == 0){//draw
        PaintInfo* pi = (PaintInfo*)wParam;
        if(pi->start_pos_ == INVALID_POS || pi->end_pos_ == INVALID_POS)
            DrawNormal(pi);
        else
            DrawRepeat(pi);
        delete pi;
    }else{//update
        PaintUpdateInfo* pui = (PaintUpdateInfo*)wParam;
        if( main_window_.taskbar_list_){
            main_window_.taskbar_list_->SetProgressValue( main_window_, pui->current_pos_, length_ );
        }
        if( pui->color_ == 0)
            UpdateNormalPlayingPosition(pui->previous_pos_, pui->current_pos_);
        else
            UpdateRepeatedPlayingPosition(pui->previous_pos_, pui->current_pos_);
        delete pui;
    }

    return TRUE;
}
void ProgressBar::DrawNormal(const PaintInfo* pi)
{
    HDC hdc = GetDC();
    Graphics gfx(hdc);
    this->DrawBackground(gfx, pi->sections_);
    this->DrawRectangle(gfx, play_brush_, 0, pi->current_pos_ );
    ReleaseDC(hdc);

}
void ProgressBar::SetLength(long length)
{
    length_ = length;
    if( main_window_.taskbar_list_){
        main_window_.taskbar_list_->SetProgressState( main_window_, TBPF_NOPROGRESS );
        main_window_.taskbar_list_->SetProgressValue( main_window_, 0, length_);
    }
}
void ProgressBar::DrawRepeat(const PaintInfo* pi)
{
    HDC hdc = GetDC();
    Graphics gfx(hdc);
    this->DrawBackground(gfx, pi->sections_);
    this->DrawRectangle(gfx, play_brush_, pi->start_pos_, pi->end_pos_);
    this->DrawRectangle(gfx, repeat_brush_, pi->start_pos_, pi->current_pos_);
    ReleaseDC(hdc);
}
void ProgressBar::DrawBackground(Graphics& gfx, const Sections& sections)
{
    LinearGradientBrush br(rect2_, Color(10, 10, 10), Color(66, 66, 66), LinearGradientModeVertical);
    gfx.FillRectangle(&br, rect2_);

    SolidBrush p(Color(25, 25, 25));
    gfx.FillRectangle(&p, rect1_);

    gfx.FillRectangle(background_brush_, rect0_);
    for(Sections::const_iterator ci = sections.begin(); ci != sections.end(); ++ci){
         this->DrawLine(gfx, section_brush_, ci->begin_, ci->end_);
    }
}
void ProgressBar::UpdateNormalPlayingPosition(long pre_pos, long current_pos)
{
    HDC hdc = GetDC();
    Graphics gfx(hdc);
    this->DrawRectangle(gfx, play_brush_, pre_pos, current_pos);
    
    ReleaseDC(hdc);

}

void ProgressBar::UpdateRepeatedPlayingPosition(long pre_pos, long new_pos)
{
    HDC hdc = GetDC();
    Graphics gfx(hdc);
    this->DrawRectangle(gfx, repeat_brush_, pre_pos, new_pos);
    ReleaseDC(hdc);
}

void ProgressBar::OnPaint(CDCHandle dc)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(&ps);
    Graphics gfx(hdc);
    DrawBackground(gfx, Sections());
    EndPaint(&ps);
    main_window_.mq_.PutMessage(boost::shared_ptr<Message>(new RequestPaintInfoEvent()));

}
LRESULT ProgressBar::OnDestroy(){
    PostQuitMessage(0);
    delete play_brush_;
    delete repeat_brush_;
    delete background_brush_;
    delete section_brush_;
    
    return 0;
} 

LRESULT ProgressBar::OnSetCursor(HWND , UINT , UINT )
{
    static HCURSOR hcur = LoadCursor(NULL, IDC_HAND);
    if (NULL != hcur){
        SetCursor(hcur);
        return TRUE;
    }
    else{
        SetMsgHandled(false);
        return FALSE;
    }
    return TRUE;
}

LRESULT ProgressBar::OnButtonDown( UINT WPARAM, CPoint point)
{
    long new_pos = GetPos(point);
    if(new_pos != -1){
        main_window_.mq_.PutMessage(boost::shared_ptr<Message>(new SetPosEvent(new_pos)));
    }
    return 0;
}

void ProgressBar::DrawRectangle(Graphics& gfx, Brush* brush, long from, long to)
{
    assert(from <= length_ && from >= 0);
    assert(to <= length_ && to >= 0);
    LOG(logDEBUG1)<< __FUNCTION__ << "from = " << from << " to = " << to;
    Rect rect = rect0_;
    int from_point = CalculateX( from, length_, rect0_.Width );
    int to_point   = CalculateX( to, length_, rect0_.Width );
    rect.X += from_point;
    rect.Width =  to_point - from_point;
    gfx.FillRectangle(brush, rect);
}

void ProgressBar::DrawLine(Graphics& gfx, Brush* brush, long from, long to)
{
    assert(from <= length_ && from >= 0);
    assert(to <= length_ && to >= 0);
    Rect rect = rect0_;
    int from_point = CalculateX( from, length_, rect0_.Width );
    int to_point   = CalculateX( to, length_, rect0_.Width );
    rect.X += from_point;
    rect.Y = rect2_.Y;
    rect.Height = 2;
    rect.Width =  to_point - from_point;
    gfx.FillRectangle(brush, rect);
}
long ProgressBar::GetPos(CPoint point)
{
    if ( point.x < 0 || point.x > rect0_.Width ||
         point.y < 0 || point.y > rect0_.Height){
         LOG(logERROR) << __FUNCTION__ << " Illegal arguments";
         return -1;
    }
    return static_cast<long>(((double) (point.x - 2 )/rect0_.Width) * length_);
}